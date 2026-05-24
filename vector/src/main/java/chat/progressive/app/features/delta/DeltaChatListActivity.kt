package chat.progressive.app.features.delta

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import chat.progressive.app.delta.DeltaSessionHolder
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.R
import im.vector.app.databinding.ActivityDeltaChatListBinding
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale
import javax.inject.Inject

@AndroidEntryPoint
class DeltaChatListActivity : AppCompatActivity() {

    @Inject lateinit var deltaSessionHolder: DeltaSessionHolder

    private lateinit var binding: ActivityDeltaChatListBinding
    private lateinit var adapter: ChatAdapter

    companion object {
        fun newIntent(context: Context): Intent {
            return Intent(context, DeltaChatListActivity::class.java)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityDeltaChatListBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.deltaChatList.layoutManager = LinearLayoutManager(this)
        adapter = ChatAdapter { chatId ->
            startActivity(DeltaChatActivity.newIntent(this, chatId))
        }
        binding.deltaChatList.adapter = adapter

        binding.deltaContactsButton.setOnClickListener {
            startActivity(DeltaContactsActivity.newIntent(this))
        }

        binding.deltaNewGroupButton.setOnClickListener {
            showCreateGroupDialog()
        }

        loadChats()
    }

    override fun onResume() {
        super.onResume()
        loadChats()
    }

    private fun loadChats() {
        val session = deltaSessionHolder.getSession() ?: return
        val chats = session.getChats()
        adapter.setChats(chats)
    }

    private fun showCreateGroupDialog() {
        val input = EditText(this).apply {
            hint = "Group name"
        }
        AlertDialog.Builder(this)
            .setTitle("Create Group")
            .setView(input)
            .setPositiveButton("Create") { _, _ ->
                val name = input.text.toString().trim()
                if (name.isNotEmpty()) {
                    deltaSessionHolder.getSession()?.createGroupChat(false, name)
                    loadChats()
                    Toast.makeText(this, "Group created", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private class ChatAdapter(
        private val onChatClick: (Long) -> Unit
    ) : RecyclerView.Adapter<ChatAdapter.ViewHolder>() {

        private var chats = listOf<chat.progressive.app.protocol.IProtocolRoom>()

        fun setChats(chats: List<chat.progressive.app.protocol.IProtocolRoom>) {
            this.chats = chats
            notifyDataSetChanged()
        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
            val view = LayoutInflater.from(parent.context)
                .inflate(R.layout.item_delta_chat, parent, false)
            return ViewHolder(view)
        }

        override fun onBindViewHolder(holder: ViewHolder, position: Int) {
            val chat = chats[position]
            holder.name.text = chat.title
            holder.message.text = chat.lastMessage ?: ""
            holder.time.text = chat.lastMessageTime?.let {
                SimpleDateFormat("HH:mm", Locale.getDefault()).format(Date(it * 1000))
            } ?: ""

            val unread = chat.unreadCount
            if (unread > 0) {
                holder.unread.visibility = View.VISIBLE
                holder.unread.text = if (unread > 99) "99+" else unread.toString()
            } else {
                holder.unread.visibility = View.GONE
            }

            holder.itemView.setOnClickListener { onChatClick(chat.id) }
        }

        override fun getItemCount() = chats.size

        class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {
            val name: TextView = view.findViewById(R.id.deltaChatItemName)
            val message: TextView = view.findViewById(R.id.deltaChatItemMessage)
            val time: TextView = view.findViewById(R.id.deltaChatItemTime)
            val unread: TextView = view.findViewById(R.id.deltaChatItemUnread)
        }
    }
}
