package chat.progressive.app.features.lemmy.home

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import im.vector.app.databinding.ItemInboxBinding

class LemmyCommunityAdapter(
    private val onClick: (LemmyCommunityItem) -> Unit,
    private val onSubscribeClick: (LemmyCommunityItem) -> Unit
) : RecyclerView.Adapter<LemmyCommunityAdapter.Holder>() {

    private var communities: List<LemmyCommunityItem> = emptyList()

    fun submitList(list: List<LemmyCommunityItem>) {
        communities = list
        notifyDataSetChanged()
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): Holder {
        return Holder(ItemInboxBinding.inflate(LayoutInflater.from(parent.context), parent, false))
    }

    override fun onBindViewHolder(holder: Holder, position: Int) {
        holder.bind(communities[position])
    }

    override fun getItemCount(): Int = communities.size

    inner class Holder(private val binding: ItemInboxBinding) : RecyclerView.ViewHolder(binding.root) {
        fun bind(community: LemmyCommunityItem) {
            binding.inboxType.text = "${community.subscriberCount} subs"
            binding.inboxSender.text = community.title
            binding.inboxContent.text = community.description ?: community.name
            binding.inboxContext.text = "!${community.name}"
            binding.inboxTime.text = "${community.postCount} posts"
            binding.inboxUnreadDot.visibility = android.view.View.GONE

            binding.root.setOnClickListener { onClick(community) }
        }
    }
}
