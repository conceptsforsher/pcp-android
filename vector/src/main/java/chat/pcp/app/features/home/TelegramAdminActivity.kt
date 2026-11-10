package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.Toast
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding

class TelegramAdminActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        private const val EXTRA_CHAT_ID = "chat_id"
        fun newIntent(context: Context, chatId: Long): Intent =
            Intent(context, TelegramAdminActivity::class.java).putExtra(EXTRA_CHAT_ID, chatId)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val chatId = intent.getLongExtra(EXTRA_CHAT_ID, 0)
        supportActionBar?.apply {
            title = "Admin Log"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        addButton(container, "Get Administrators") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatAdministrators(h, chatId)
            Toast.makeText(this, "Loading administrators...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Search Members") {
            val input = android.widget.EditText(this).apply { hint = "Search query" }
            androidx.appcompat.app.AlertDialog.Builder(this)
                .setTitle("Search Members")
                .setView(input)
                .setPositiveButton("Search") { _, _ ->
                    val q = input.text.toString().trim()
                    if (q.isNotBlank()) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgSearchChatMembers(h, chatId, q, 50)
                        Toast.makeText(this, "Searching: $q", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Get Member Info") {
            val input = android.widget.EditText(this).apply {
                hint = "User ID"
                inputType = android.text.InputType.TYPE_CLASS_NUMBER
            }
            androidx.appcompat.app.AlertDialog.Builder(this)
                .setTitle("Get Member")
                .setView(input)
                .setPositiveButton("Get") { _, _ ->
                    val uid = input.text.toString().toLongOrNull() ?: 0
                    if (uid > 0) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgGetChatMember(h, chatId, uid)
                        Toast.makeText(this, "Loading member $uid...", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Block User") {
            val input = android.widget.EditText(this).apply {
                hint = "User ID"
                inputType = android.text.InputType.TYPE_CLASS_NUMBER
            }
            androidx.appcompat.app.AlertDialog.Builder(this)
                .setTitle("Block User")
                .setView(input)
                .setPositiveButton("Block") { _, _ ->
                    val uid = input.text.toString().toLongOrNull() ?: 0
                    if (uid > 0) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgBlockUser(h, uid)
                        Toast.makeText(this, "User blocked", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Unblock User") {
            val input = android.widget.EditText(this).apply {
                hint = "User ID"
                inputType = android.text.InputType.TYPE_CLASS_NUMBER
            }
            androidx.appcompat.app.AlertDialog.Builder(this)
                .setTitle("Unblock User")
                .setView(input)
                .setPositiveButton("Unblock") { _, _ ->
                    val uid = input.text.toString().toLongOrNull() ?: 0
                    if (uid > 0) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgUnblockUser(h, uid)
                        Toast.makeText(this, "User unblocked", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
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
