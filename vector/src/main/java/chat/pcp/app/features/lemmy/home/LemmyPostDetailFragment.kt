package chat.pcp.app.features.lemmy.home

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.LinearLayoutManager
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.core.platform.VectorBaseFragment
import im.vector.app.databinding.FragmentLemmyPostDetailBinding

@AndroidEntryPoint
class LemmyPostDetailFragment :
    VectorBaseFragment<FragmentLemmyPostDetailBinding>() {

    private val viewModel: LemmyPostDetailViewModel by fragmentViewModel()
    private lateinit var commentAdapter: LemmyCommentAdapter

    companion object {
        const val ARG_POST_ID = "post_id"
        const val ARG_POST_TITLE = "post_title"
        const val ARG_POST_BODY = "post_body"
        const val ARG_POST_COMMUNITY = "post_community"
        const val ARG_POST_CREATOR = "post_creator"
        const val ARG_POST_SCORE = "post_score"
        const val ARG_POST_COMMENTS = "post_comments"
        const val ARG_POST_URL = "post_url"
        const val ARG_POST_MYVOTE = "post_myvote"

        fun newInstance(post: LemmyPostItem): LemmyPostDetailFragment {
            return LemmyPostDetailFragment().apply {
                arguments = Bundle().apply {
                    putInt(ARG_POST_ID, post.id)
                    putString(ARG_POST_TITLE, post.title)
                    putString(ARG_POST_BODY, post.body)
                    putString(ARG_POST_COMMUNITY, post.communityName)
                    putString(ARG_POST_CREATOR, post.creatorName)
                    putInt(ARG_POST_SCORE, post.score)
                    putInt(ARG_POST_COMMENTS, post.commentCount)
                    putString(ARG_POST_URL, post.url)
                    putInt(ARG_POST_MYVOTE, post.myVote)
                }
            }
        }
    }

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentLemmyPostDetailBinding {
        return FragmentLemmyPostDetailBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        setupToolbar(views.toolbar).allowBack()
        views.toolbar.title = arguments?.getString(ARG_POST_TITLE) ?: "Post"

        commentAdapter = LemmyCommentAdapter(
            onPostVoteClick = { score -> viewModel.handle(LemmyPostDetailAction.VotePost(score)) },
            onCommentVoteClick = { id, score -> viewModel.handle(LemmyPostDetailAction.VoteComment(id, score)) },
            onReplyClick = { id -> viewModel.handle(LemmyPostDetailAction.ReplyToComment(id)) }
        )

        views.list.layoutManager = LinearLayoutManager(requireContext())
        views.list.adapter = commentAdapter

        // Reply bar
        views.replySend.setOnClickListener {
            viewModel.handle(LemmyPostDetailAction.SendReply)
        }

        // Observe state
        viewModel.onEach(LemmyPostDetailState::comments) { comments ->
            val post = withState(viewModel) { it.post }
            commentAdapter.submitList(post, comments)
        }

        viewModel.onEach(LemmyPostDetailState::replyText) { text ->
            if (views.replyInput.text.toString() != text) {
                views.replyInput.setText(text)
                if (text.isNotEmpty()) {
                    views.replyInput.setSelection(text.length)
                }
            }
        }

        viewModel.onEach(LemmyPostDetailState::error) { error ->
            if (error != null) showErrorInSnackbar(Exception(error))
        }

        viewModel.observeViewEvents { event ->
            when (event) {
                is LemmyHomeViewEvents.Failure -> showErrorInSnackbar(event.throwable)
                else -> {}
            }
        }
    }
}
