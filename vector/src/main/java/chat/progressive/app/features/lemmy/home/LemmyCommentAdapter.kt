package chat.progressive.app.features.lemmy.home

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.recyclerview.widget.RecyclerView
import im.vector.app.databinding.ItemCommentBinding
import im.vector.app.databinding.ItemPostBinding

class LemmyCommentAdapter(
    private val onPostVoteClick: (Int) -> Unit,
    private val onCommentVoteClick: (Int, Int) -> Unit,
    private val onReplyClick: (Int) -> Unit
) : RecyclerView.Adapter<RecyclerView.ViewHolder>() {

    companion object {
        private const val TYPE_POST_HEADER = 0
        private const val TYPE_COMMENT = 1
    }

    private var comments: List<LemmyCommentItem> = emptyList()
    private var post: LemmyPostItem? = null

    fun submitList(post: LemmyPostItem?, newComments: List<LemmyCommentItem>) {
        this.post = post
        this.comments = newComments
        notifyDataSetChanged()
    }

    override fun getItemViewType(position: Int): Int {
        return if (position == 0 && post != null) TYPE_POST_HEADER else TYPE_COMMENT
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
        return when (viewType) {
            TYPE_POST_HEADER -> {
                val binding = im.vector.app.databinding.ItemPostBinding.inflate(
                    LayoutInflater.from(parent.context), parent, false)
                PostHeaderHolder(binding)
            }
            else -> {
                val binding = ItemCommentBinding.inflate(
                    LayoutInflater.from(parent.context), parent, false)
                Holder(binding)
            }
        }
    }

    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {
        if (holder is PostHeaderHolder && post != null) {
            holder.bind(post!!)
        } else if (holder is Holder) {
            val offset = if (post != null) 1 else 0
            holder.bind(comments[position - offset])
        }
    }

    override fun getItemCount(): Int = (if (post != null) 1 else 0) + comments.size

    inner class PostHeaderHolder(private val binding: im.vector.app.databinding.ItemPostBinding) :
        RecyclerView.ViewHolder(binding.root) {
        fun bind(post: LemmyPostItem) {
            binding.postTitle.text = post.title
            binding.postCommunity.text = "!${post.communityName}"
            binding.postCreator.text = post.creatorName
            binding.postScore.text = "${post.score}"
            binding.postComments.text = "${post.commentCount} comments"
            binding.postBody.text = if (post.body.isNotEmpty()) LemmyMarkdown.render(post.body) else null
            binding.postBody.visibility = if (post.body.isNotEmpty()) android.view.View.VISIBLE else android.view.View.GONE
            binding.postUrl.text = post.url
            binding.postUrl.visibility = if (!post.url.isNullOrEmpty()) android.view.View.VISIBLE else android.view.View.GONE

            val upColor = if (post.myVote == 1) 0xFF00C853.toInt() else 0xFF888888.toInt()
            val downColor = if (post.myVote == -1) 0xFFFF4444.toInt() else 0xFF888888.toInt()
            binding.postUpvote.setTextColor(upColor)
            binding.postDownvote.setTextColor(downColor)

            binding.postUpvote.setOnClickListener { onPostVoteClick(if (post.myVote == 1) 0 else 1) }
            binding.postDownvote.setOnClickListener { onPostVoteClick(if (post.myVote == -1) 0 else -1) }
        }
    }

    inner class Holder(private val binding: ItemCommentBinding) : RecyclerView.ViewHolder(binding.root) {
        fun bind(comment: LemmyCommentItem) {
            // Indent by depth
            val indent = (comment.depth * 16).coerceAtMost(80)
            binding.root.setPadding(
                (12 + indent).dpToPx(binding.root.context),
                8.dpToPx(binding.root.context),
                12.dpToPx(binding.root.context),
                8.dpToPx(binding.root.context)
            )

            binding.commentAuthor.text = comment.authorName
            binding.commentContent.text = comment.content
            binding.commentScore.text = "${comment.score}"

            val upColor = if (comment.myVote == 1) 0xFF00C853.toInt() else 0xFF888888.toInt()
            val downColor = if (comment.myVote == -1) 0xFFFF4444.toInt() else 0xFF888888.toInt()
            binding.commentUpvote.setTextColor(upColor)
            binding.commentDownvote.setTextColor(downColor)

            binding.commentUpvote.setOnClickListener {
                onCommentVoteClick(comment.id, if (comment.myVote == 1) 0 else 1)
            }
            binding.commentDownvote.setOnClickListener {
                onCommentVoteClick(comment.id, if (comment.myVote == -1) 0 else -1)
            }
            binding.commentReply.setOnClickListener { onReplyClick(comment.id) }
        }
    }

    private fun Int.dpToPx(context: android.content.Context): Int =
        (this * context.resources.displayMetrics.density).toInt()
}
