package chat.progressive.app.features.xmpp

import android.os.Bundle
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.LinearLayout
import android.widget.ListView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import chat.progressive.app.protocol.IProtocolRoom
import chat.progressive.app.protocol.ProtocolManager
import chat.progressive.app.protocol.ProtocolType
import dagger.hilt.android.AndroidEntryPoint
import javax.inject.Inject

@AndroidEntryPoint
class XmppChatListActivity : AppCompatActivity() {

    @Inject
    lateinit var protocolManager: ProtocolManager

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
            text = "Unified Chats (Matrix + XMPP)"
            textSize = 20f
        }
        layout.addView(title)

        val filterBar = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL
            setPadding(0, 8, 0, 8)
        }
        val allBtn = Button(this).apply {
            text = "All"
            setOnClickListener { chatItems.forEachIndexed { idx, s ->
                if (!s.startsWith("[")) chatItems[idx] = s.substringAfter("]", "").trim()
            }; updateList(chats) }
        }
        filterBar.addView(allBtn)

        val matrixBtn = Button(this).apply {
            text = "Matrix"
            setOnClickListener { updateList(chats.filter { it.protocolType == ProtocolType.MATRIX }) }
        }
        filterBar.addView(matrixBtn)

        val xmppBtn = Button(this).apply {
            text = "XMPP"
            setOnClickListener { updateList(chats.filter { it.protocolType == ProtocolType.XMPP }) }
        }
        filterBar.addView(xmppBtn)
        layout.addView(filterBar)

        val listView = ListView(this)
        adapter = ArrayAdapter(this, android.R.layout.simple_list_item_2, android.R.id.text1, chatItems)
        listView.adapter = adapter

        listView.setOnItemClickListener { _, _, position, _ ->
            if (position < chats.size) {
                val room = chats[position]
                Toast.makeText(this@XmppChatListActivity,
                    "Open: ${room.title} (${room.protocolType.displayName})", Toast.LENGTH_SHORT).show()
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
        chats = protocolManager.getUnifiedChats()
        updateList(chats)
    }

    private fun updateList(items: List<IProtocolRoom>) {
        chats = items
        chatItems.clear()
        items.forEach { room ->
            val prefix = "[${room.protocolType.displayName}]"
            val unread = if (room.unreadCount > 0) " (${room.unreadCount})" else ""
            chatItems.add("$prefix ${room.title}$unread")
        }
        if (chatItems.isEmpty()) {
            chatItems.add("No chats — connect a protocol first")
        }
        adapter.notifyDataSetChanged()
    }

    override fun onResume() {
        super.onResume()
        refreshChats()
    }
}
