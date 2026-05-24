package chat.progressive.app.features.lemmy.home

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.LinearLayoutManager
import com.airbnb.mvrx.fragmentViewModel
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.core.platform.StateView
import im.vector.app.core.platform.VectorBaseFragment
import im.vector.app.databinding.FragmentLemmyInboxBinding

@AndroidEntryPoint
class LemmyInboxFragment :
    VectorBaseFragment<FragmentLemmyInboxBinding>() {

    private val viewModel: LemmyInboxViewModel by fragmentViewModel()
    private lateinit var adapter: LemmyInboxAdapter
    private var selectedTab: Int = 0

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentLemmyInboxBinding {
        return FragmentLemmyInboxBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        setupToolbar(views.toolbar).allowBack()

        adapter = LemmyInboxAdapter(
            onItemClick = { item ->
                viewModel.handle(LemmyInboxAction.MarkRead(item))
            }
        )

        views.stateView.contentView = views.inboxList
        views.inboxList.layoutManager = LinearLayoutManager(requireContext())
        views.inboxList.adapter = adapter

        views.tabReplies.setOnClickListener { selectTab(0) }
        views.tabMentions.setOnClickListener { selectTab(1) }
        views.tabMessages.setOnClickListener { selectTab(2) }
        views.markAllRead.setOnClickListener { viewModel.handle(LemmyInboxAction.MarkAllRead) }

        // Observe state
        viewModel.onEach(LemmyInboxState::loading) { loading ->
            if (loading) views.stateView.state = StateView.State.Loading
        }

        viewModel.onEach(LemmyInboxState::replies) { updateList() }
        viewModel.onEach(LemmyInboxState::mentions) { updateList() }
        viewModel.onEach(LemmyInboxState::messages) { updateList() }

        viewModel.onEach(LemmyInboxState::unreadCount) { count ->
            views.toolbar.title = if (count > 0) "Inbox ($count)" else "Inbox"
        }

        viewModel.onEach(LemmyInboxState::error) { error ->
            if (error != null && adapter.itemCount == 0) {
                views.stateView.state = StateView.State.Empty(
                    title = "Inbox",
                    message = error
                )
            }
        }

        viewModel.observeViewEvents { event ->
            when (event) {
                is LemmyHomeViewEvents.Failure -> showErrorInSnackbar(event.throwable)
                else -> {}
            }
        }

        viewModel.handle(LemmyInboxAction.Refresh)
    }

    private fun selectTab(tab: Int) {
        selectedTab = tab
        updateList()
    }

    private fun updateList() {
        val items = withState(viewModel) { state ->
            when (selectedTab) {
                0 -> state.replies
                1 -> state.mentions
                2 -> state.messages
                else -> emptyList()
            }
        }
        if (items.isNotEmpty()) {
            views.stateView.state = StateView.State.Content
            adapter.submitList(items)
        } else if (!withState(viewModel) { it.loading }) {
            views.stateView.state = StateView.State.Empty(
                title = "No items",
                message = "Pull to refresh"
            )
        }
    }
}
