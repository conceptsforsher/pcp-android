package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding

class TelegramJoinRequestsActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        private const val EXTRA_CHAT_ID = "chat_id"
        fun newIntent(context: Context, chatId: Long): Intent =
            Intent(context, TelegramJoinRequestsActivity::class.java).putExtra(EXTRA_CHAT_ID, chatId)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val chatId = intent.getLongExtra(EXTRA_CHAT_ID, 0)
        supportActionBar?.apply {
            title = "Join Requests"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        addButton(container, "Check Invite Link") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatInviteLink(h, chatId)
            Toast.makeText(this, "Loading invite link...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Join by Link") {
            val input = EditText(this).apply { hint = "t.me/joinchat/..." }
            AlertDialog.Builder(this)
                .setTitle("Join via Link")
                .setView(input)
                .setPositiveButton("Join") { _, _ ->
                    val link = input.text.toString().trim()
                    if (link.isNotBlank()) {
                        PcpNative.tgJoinChatByInviteLink(NativeSessionManager.getHandle(), link)
                        Toast.makeText(this, "Joining...", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Check Link Info") {
            val input = EditText(this).apply { hint = "t.me/..." }
            AlertDialog.Builder(this)
                .setTitle("Check Invite Link")
                .setView(input)
                .setPositiveButton("Check") { _, _ ->
                    val link = input.text.toString().trim()
                    if (link.isNotBlank()) {
                        PcpNative.tgCheckChatInviteLink(NativeSessionManager.getHandle(), link)
                        Toast.makeText(this, "Checking link info...", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Create Invite Link") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatInviteLink(h, chatId)
            Toast.makeText(this, "Creating invite link...", Toast.LENGTH_SHORT).show()
        }

        val scrollView = android.widget.ScrollView(this).apply { addView(container) }
        views.telegramMsgRecycler.visibility = android.view.View.GONE
        val root = findViewById<android.view.ViewGroup>(android.R.id.content)
        root.addView(scrollView, android.view.ViewGroup.LayoutParams(
            android.view.ViewGroup.LayoutParams.MATCH_PARENT,
            android.view.ViewGroup.LayoutParams.MATCH_PARENT
        ))
    }

    private fun addButton(container: android.widget.LinearLayout, text: String, onClick: () -> Unit) {
        val btn = android.widget.Button(this).apply {
            this.text = text
            setTextColor(0xFF000000.toInt())
            setOnClickListener { onClick() }
        }
        container.addView(btn)
    }
}
