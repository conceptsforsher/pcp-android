package chat.pcp.app.features.lemmy.home

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.LinearLayoutManager
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.core.platform.StateView
import im.vector.app.core.platform.VectorBaseFragment
import im.vector.app.databinding.FragmentLemmyHomeBinding

@AndroidEntryPoint
class LemmyHomeFragment :
    VectorBaseFragment<FragmentLemmyHomeBinding>() {

    private val viewModel: LemmyHomeViewModel by fragmentViewModel()
    private lateinit var postAdapter: LemmyPostAdapter
    private lateinit var communityAdapter: LemmyCommunityAdapter

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
        communityAdapter = LemmyCommunityAdapter(
            onClick = { viewModel.handle(LemmyHomeAction.SelectCommunity(it)) },
            onSubscribeClick = { viewModel.handle(LemmyHomeAction.SelectCommunity(it)) }
        )

        views.stateView.contentView = views.postList
        views.postList.layoutManager = LinearLayoutManager(requireContext())
        views.postList.adapter = postAdapter

        // Tab bar
        views.tabPosts.setOnClickListener { viewModel.handle(LemmyHomeAction.SwitchTab(0)) }
        views.tabCommunities.setOnClickListener { viewModel.handle(LemmyHomeAction.SwitchTab(1)) }

        // State observers
        viewModel.onEach { state ->
            if (state.loading) {
                views.stateView.state = StateView.State.Loading
                return@onEach
            }

            if (state.selectedTab == 0) {
                views.postList.adapter = postAdapter
                if (state.posts.isNotEmpty()) {
                    views.stateView.state = StateView.State.Content
                    postAdapter.submitList(state.posts)
                } else {
                    views.stateView.state = StateView.State.Empty(
                        title = "No posts",
                        message = state.error ?: "Subscribe to communities"
                    )
                }
            } else {
                views.postList.adapter = communityAdapter
                if (state.communities.isNotEmpty()) {
                    views.stateView.state = StateView.State.Content
                    communityAdapter.submitList(state.communities)
                } else {
                    views.stateView.state = StateView.State.Empty(
                        title = "No communities",
                        message = state.error ?: "Connect from Settings"
                    )
                }
            }
        }

        // Events
        viewModel.observeViewEvents { event ->
            when (event) {
                is LemmyHomeViewEvents.Failure -> showErrorInSnackbar(event.throwable)
                is LemmyHomeViewEvents.OpenPost -> {
                    val fragment = LemmyPostDetailFragment.newInstance(event.post)
                    parentFragmentManager.beginTransaction()
                        .replace(im.vector.app.R.id.roomListContainer, fragment)
                        .addToBackStack(null)
                        .commit()
                }
                is LemmyHomeViewEvents.OpenCommunity -> {
                    // TODO: show community posts
                }
            }
        }

        viewModel.handle(LemmyHomeAction.Refresh)
    }
}
