package chat.progressive.app.features.lemmy.home

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.recyclerview.widget.RecyclerView
import im.vector.app.databinding.ItemInboxBinding

class LemmyInboxAdapter(
    private val onItemClick: (LemmyInboxItem) -> Unit
) : RecyclerView.Adapter<LemmyInboxAdapter.Holder>() {

    private var items: List<LemmyInboxItem> = emptyList()

    fun submitList(newItems: List<LemmyInboxItem>) {
        items = newItems
        notifyDataSetChanged()
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): Holder {
        return Holder(ItemInboxBinding.inflate(LayoutInflater.from(parent.context), parent, false))
    }

    override fun onBindViewHolder(holder: Holder, position: Int) {
        holder.bind(items[position])
    }

    override fun getItemCount(): Int = items.size

    inner class Holder(private val binding: ItemInboxBinding) : RecyclerView.ViewHolder(binding.root) {
        fun bind(item: LemmyInboxItem) {
            binding.inboxType.text = when (item.type) {
                InboxType.REPLY -> "REPLY"
                InboxType.MENTION -> "MENTION"
                InboxType.MESSAGE -> "MESSAGE"
            }
            binding.inboxSender.text = item.senderName
            binding.inboxContent.text = item.content
            binding.inboxContext.text = when {
                item.communityName.isNotEmpty() -> "!${item.communityName} · ${item.postName}"
                else -> item.postName
            }.takeIf { it.isNotBlank() } ?: ""
            binding.inboxTime.text = item.published.takeLast(16)
            binding.inboxUnreadDot.isVisible = !item.isRead

            @Suppress("DEPRECATION")
            binding.root.alpha = if (item.isRead) 0.5f else 1.0f
            binding.root.setOnClickListener { onItemClick(item) }
        }
    }
}
