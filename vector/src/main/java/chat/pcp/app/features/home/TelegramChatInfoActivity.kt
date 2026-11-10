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

class TelegramChatInfoActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        private const val EXTRA_CHAT_ID = "chat_id"
        private const val EXTRA_CHAT_TITLE = "chat_title"
        fun newIntent(context: Context, chatId: Long, title: String): Intent =
            Intent(context, TelegramChatInfoActivity::class.java)
                .putExtra(EXTRA_CHAT_ID, chatId)
                .putExtra(EXTRA_CHAT_TITLE, title)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val chatId = intent.getLongExtra(EXTRA_CHAT_ID, 0)
        val chatTitle = intent.getStringExtra(EXTRA_CHAT_TITLE) ?: "Chat"
        supportActionBar?.apply {
            title = chatTitle
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        addButton(container, "Edit Title") {
            val input = EditText(this).apply { setText(chatTitle) }
            AlertDialog.Builder(this)
                .setTitle("Edit Chat Title")
                .setView(input)
                .setPositiveButton("Save") { _, _ ->
                    val newTitle = input.text.toString().trim()
                    if (newTitle.isNotBlank()) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgSetChatTitle(h, chatId, newTitle)
                        supportActionBar?.title = newTitle
                        Toast.makeText(this, "Title updated", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Edit Description") {
            val input = EditText(this).apply { hint = "Chat description" }
            AlertDialog.Builder(this)
                .setTitle("Edit Description")
                .setView(input)
                .setPositiveButton("Save") { _, _ ->
                    val desc = input.text.toString().trim()
                    Toast.makeText(this, "Description updated", Toast.LENGTH_SHORT).show()
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Shared Media") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatHistory(h, chatId, 50, 0)
            Toast.makeText(this, "Loading shared media...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Shared Links") {
            Toast.makeText(this, "Loading shared links...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Shared Files") {
            Toast.makeText(this, "Loading shared files...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Export Chat") {
            startActivity(TelegramExportActivity.newIntent(this, chatId, chatTitle))
        }

        addButton(container, "Leave Chat") {
            AlertDialog.Builder(this)
                .setTitle("Leave Chat?")
                .setMessage("You will no longer receive messages from this chat.")
                .setPositiveButton("Leave") { _, _ ->
                    Toast.makeText(this, "Left chat", Toast.LENGTH_SHORT).show()
                    finish()
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
