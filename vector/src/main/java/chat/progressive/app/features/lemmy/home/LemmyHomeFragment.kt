package chat.progressive.app.features.lemmy.home

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import com.airbnb.mvrx.fragmentViewModel
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.core.platform.StateView
import im.vector.app.core.platform.VectorBaseFragment
import im.vector.app.databinding.FragmentLemmyHomeBinding
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

@AndroidEntryPoint
class LemmyHomeFragment :
    VectorBaseFragment<FragmentLemmyHomeBinding>() {

    private val viewModel: LemmyHomeViewModel by fragmentViewModel()
    private lateinit var postAdapter: LemmyPostAdapter

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentLemmyHomeBinding {
        return FragmentLemmyHomeBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        setupToolbar(views.toolbar).allowBack()
        views.toolbar.inflateMenu(im.vector.app.R.menu.home_bottom_navigation)
        views.toolbar.setOnMenuItemClickListener { item ->
            when (item.itemId) {
                im.vector.app.R.id.bottom_action_lemmy -> {
                    parentFragmentManager.beginTransaction()
                        .replace(im.vector.app.R.id.roomListContainer, LemmyInboxFragment())
                        .addToBackStack(null)
                        .commit()
                    true
                }
                else -> false
            }
        }

        postAdapter = LemmyPostAdapter(
            onPostClick = { viewModel.handle(LemmyHomeAction.SelectPost(it)) },
            onVoteClick = { postId, score -> viewModel.handle(LemmyHomeAction.VotePost(postId, score)) }
        )

        views.stateView.contentView = views.postList
        views.postList.layoutManager = LinearLayoutManager(requireContext())
        views.postList.adapter = postAdapter

        // Tab bar
        views.tabPosts.setOnClickListener { viewModel.handle(LemmyHomeAction.SwitchTab(0)) }
        views.tabCommunities.setOnClickListener { viewModel.handle(LemmyHomeAction.SwitchTab(1)) }

        // Observe state
        viewModel.onEach(LemmyHomeViewState::loading) { loading ->
            if (loading) views.stateView.state = StateView.State.Loading
        }

        viewModel.onEach(LemmyHomeViewState::posts) { posts ->
            if (posts.isNotEmpty()) {
                views.stateView.state = StateView.State.Content
                postAdapter.submitList(posts)
            }
        }

        viewModel.onEach(LemmyHomeViewState::error) { error ->
            if (error != null && postAdapter.itemCount == 0) {
                views.stateView.state = StateView.State.Empty(
                    title = "Lemmy",
                    message = error
                )
            }
        }

        // Observe events
        viewModel.observeViewEvents { event ->
            when (event) {
                is LemmyHomeViewEvents.Failure -> {
                    showErrorInSnackbar(event.throwable)
                }
                is LemmyHomeViewEvents.OpenPost -> {
                    val fragment = LemmyPostDetailFragment.newInstance(event.post)
                    parentFragmentManager.beginTransaction()
                        .replace(im.vector.app.R.id.roomListContainer, fragment)
                        .addToBackStack(null)
                        .commit()
                }
                is LemmyHomeViewEvents.OpenCommunity -> {
                    // TODO: open community detail
                }
            }
        }

        // Trigger initial load
        viewModel.handle(LemmyHomeAction.Refresh)
    }
}
