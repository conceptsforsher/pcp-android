package chat.progressive.app.features.xmpp

import android.os.Bundle
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.ListView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import chat.progressive.app.protocol.IProtocolRoom
import chat.progressive.app.xmpp.XmppSessionHolder
import dagger.hilt.android.AndroidEntryPoint
import javax.inject.Inject

@AndroidEntryPoint
class XmppChatListActivity : AppCompatActivity() {

    @Inject
    lateinit var xmppHolder: XmppSessionHolder

    private val chatItems = mutableListOf<String>()
    private lateinit var adapter: ArrayAdapter<String>
    private var chats: List<IProtocolRoom> = emptyList()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val layout = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(16, 48, 16, 16)
        }

        val title = TextView(this).apply {
            text = "XMPP Chats"
            textSize = 24f
        }
        layout.addView(title)

        val listView = ListView(this)
        adapter = ArrayAdapter(this, android.R.layout.simple_list_item_1, chatItems)
        listView.adapter = adapter

        listView.setOnItemClickListener { _, _, position, _ ->
            if (position < chatItems.size) {
                // Navigate to chat
            }
        }

        layout.addView(listView)

        val refreshBtn = Button(this).apply {
            text = "Refresh"
            setOnClickListener { refreshChats() }
        }
        layout.addView(refreshBtn)

        setContentView(layout)

        refreshChats()
    }

    private fun refreshChats() {
        val session = xmppHolder.getSession()
        if (session != null) {
            chats = session.getChats()
            chatItems.clear()
            chats.forEach { room ->
                chatItems.add("${room.title}${if (room.unreadCount > 0) " (${room.unreadCount})" else ""}")
            }
            if (chatItems.isEmpty()) {
                chatItems.add("No chats yet")
            }
            adapter.notifyDataSetChanged()
        } else {
            chatItems.clear()
            chatItems.add("Not connected")
            adapter.notifyDataSetChanged()
        }
    }

    override fun onResume() {
        super.onResume()
        refreshChats()
    }
}
