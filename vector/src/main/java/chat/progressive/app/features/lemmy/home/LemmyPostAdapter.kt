package chat.progressive.app.features.lemmy.home

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.recyclerview.widget.RecyclerView
import im.vector.app.databinding.ItemPostBinding

class LemmyPostAdapter(
    private val onPostClick: (LemmyPostItem) -> Unit,
    private val onVoteClick: (Int, Int) -> Unit
) : RecyclerView.Adapter<LemmyPostAdapter.Holder>() {

    private var posts: List<LemmyPostItem> = emptyList()

    fun submitList(newPosts: List<LemmyPostItem>) {
        posts = newPosts
        notifyDataSetChanged()
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): Holder {
        val binding = ItemPostBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        return Holder(binding)
    }

    override fun onBindViewHolder(holder: Holder, position: Int) {
        holder.bind(posts[position])
    }

    override fun getItemCount(): Int = posts.size

    inner class Holder(private val binding: ItemPostBinding) : RecyclerView.ViewHolder(binding.root) {
        fun bind(post: LemmyPostItem) {
            binding.postTitle.text = post.title
            binding.postCommunity.text = "!${post.communityName}"
            binding.postCreator.text = post.creatorName
            binding.postScore.text = "${post.score}"
            binding.postComments.text = "${post.commentCount} comments"
            binding.postBody.text = post.body.takeIf { it.isNotEmpty() }
            binding.postBody.isVisible = !post.body.isNullOrEmpty()
            binding.postUrl.text = post.url
            binding.postUrl.isVisible = !post.url.isNullOrEmpty()

            // Vote colors
            val upColor = if (post.myVote == 1) 0xFF00C853.toInt() else 0xFF888888.toInt()
            val downColor = if (post.myVote == -1) 0xFFFF4444.toInt() else 0xFF888888.toInt()
            binding.postUpvote.setTextColor(upColor)
            binding.postDownvote.setTextColor(downColor)

            binding.root.setOnClickListener { onPostClick(post) }
            binding.postUpvote.setOnClickListener { onVoteClick(post.id, if (post.myVote == 1) 0 else 1) }
            binding.postDownvote.setOnClickListener { onVoteClick(post.id, if (post.myVote == -1) 0 else -1) }
        }
    }
}
