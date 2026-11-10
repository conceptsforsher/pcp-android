package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding

class TelegramMembersActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        private const val EXTRA_CHAT_ID = "chat_id"
        fun newIntent(context: Context, chatId: Long): Intent =
            Intent(context, TelegramMembersActivity::class.java).putExtra(EXTRA_CHAT_ID, chatId)
    }

    private var chatId: Long = 0

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        chatId = intent.getLongExtra(EXTRA_CHAT_ID, 0)
        supportActionBar?.apply {
            title = "Members"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 16, 16, 16)
        }

        // Invite Link section
        val inviteLabel = TextView(this).apply {
            text = "Invite Link"
            textSize = 16f
            setTextColor(0xFF000000.toInt())
            setPadding(0, 8, 0, 8)
        }
        container.addView(inviteLabel)

        val inviteBtn = android.widget.Button(this).apply {
            text = "Get Invite Link"
            setOnClickListener {
                val h = NativeSessionManager.getHandle()
                PcpNative.tgGetChatInviteLink(h, chatId)
                Toast.makeText(context, "Invite link requested — check native response", Toast.LENGTH_SHORT).show()
            }
        }
        container.addView(inviteBtn)

        val joinByLinkBtn = android.widget.Button(this).apply {
            text = "Join by Link"
            setOnClickListener {
                val input = android.widget.EditText(context).apply { hint = "t.me/..." }
                AlertDialog.Builder(this@TelegramMembersActivity)
                    .setTitle("Join via invite link")
                    .setView(input)
                    .setPositiveButton("Join") { _, _ ->
                        val link = input.text.toString().trim()
                        if (link.isNotBlank()) {
                            PcpNative.tgJoinChatByInviteLink(NativeSessionManager.getHandle(), link)
                            Toast.makeText(context, "Joining...", Toast.LENGTH_SHORT).show()
                        }
                    }
                    .setNegativeButton("Cancel", null)
                    .show()
            }
        }
        container.addView(joinByLinkBtn)

        // Search member
        val searchLabel = TextView(this).apply {
            text = "Search Members"
            textSize = 16f
            setTextColor(0xFF000000.toInt())
            setPadding(0, 16, 0, 8)
        }
        container.addView(searchLabel)

        val searchInput = android.widget.EditText(this).apply { hint = "Search by name..." }
        container.addView(searchInput)

        val searchBtn = android.widget.Button(this).apply {
            text = "Search"
            setOnClickListener {
                val q = searchInput.text.toString().trim()
                val h = NativeSessionManager.getHandle()
                PcpNative.tgSearchChatMembers(h, chatId, q, 50)
                Toast.makeText(context, "Searching members...", Toast.LENGTH_SHORT).show()
            }
        }
        container.addView(searchBtn)

        // Administrators
        val adminBtn = android.widget.Button(this).apply {
            text = "View Administrators"
            setOnClickListener {
                val h = NativeSessionManager.getHandle()
                PcpNative.tgGetChatAdministrators(h, chatId)
                Toast.makeText(context, "Loading admins...", Toast.LENGTH_SHORT).show()
            }
        }
        container.addView(adminBtn)

        val joinRequestsBtn = android.widget.Button(this).apply {
            text = "Check Join Requests"
            setOnClickListener {
                Toast.makeText(this@TelegramMembersActivity, "Join requests are managed by TDLib updates", Toast.LENGTH_SHORT).show()
            }
        }
        container.addView(joinRequestsBtn)

        val searchPublicBtn = android.widget.Button(this).apply {
            text = "Search Public Chats"
            setOnClickListener {
                val input = android.widget.EditText(this@TelegramMembersActivity).apply { hint = "Search query" }
                AlertDialog.Builder(this@TelegramMembersActivity)
                    .setTitle("Search Public Chats")
                    .setView(input)
                    .setPositiveButton("Search") { _, _ ->
                        val q = input.text.toString().trim()
                        if (q.isNotBlank()) {
                            val h = NativeSessionManager.getHandle()
                            PcpNative.tgSearchPublicChats(h, q)
                            Toast.makeText(this@TelegramMembersActivity, "Searching: $q", Toast.LENGTH_SHORT).show()
                        }
                    }
                    .setNegativeButton("Cancel", null)
                    .show()
            }
        }
        container.addView(searchPublicBtn)

        val scrollView = android.widget.ScrollView(this).apply { addView(container) }
        views.telegramMsgRecycler.visibility = View.GONE
        val root = findViewById<ViewGroup>(android.R.id.content)
        root.addView(scrollView, ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT))
    }
}
