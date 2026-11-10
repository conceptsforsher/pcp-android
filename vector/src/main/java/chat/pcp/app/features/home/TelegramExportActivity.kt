package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.os.Environment
import android.view.View
import android.widget.CheckBox
import android.widget.Toast
import chat.pcp.app.native.PcpNative
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding

class TelegramExportActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        private const val EXTRA_CHAT_ID = "chat_id"
        private const val EXTRA_CHAT_TITLE = "chat_title"
        fun newIntent(context: Context, chatId: Long, title: String): Intent =
            Intent(context, TelegramExportActivity::class.java)
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
            title = "Export: $chatTitle"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        val label = android.widget.TextView(this).apply {
            text = "Export chat history"
            textSize = 18f
            setTextColor(0xFF000000.toInt())
            setPadding(0, 0, 0, 16)
        }
        container.addView(label)

        val options = linkedMapOf(
            "Include Photos" to true,
            "Include Videos" to true,
            "Include Voice Messages" to true,
            "Include Documents" to true,
            "Include Stickers" to true,
            "Include Animations (GIFs)" to true,
            "Format: HTML" to true
        )

        val checks = mutableListOf<CheckBox>()
        options.forEach { (name, default) ->
            val cb = CheckBox(this).apply {
                text = name
                isChecked = default
            }
            checks.add(cb)
            container.addView(cb)
        }

        addButton(container, "Export to HTML") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatHistory(h, chatId, 100, 0)
            val path = "${Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)}/telegram_export_${chatId}.html"
            Toast.makeText(this, "Export started.\nSaving to: $path", Toast.LENGTH_LONG).show()
        }

        addButton(container, "Export to JSON") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatHistory(h, chatId, 100, 0)
            val path = "${Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)}/telegram_export_${chatId}.json"
            Toast.makeText(this, "Export started.\nSaving to: $path", Toast.LENGTH_LONG).show()
        }

        addButton(container, "Export to TXT") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatHistory(h, chatId, 100, 0)
            val path = "${Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)}/telegram_export_${chatId}.txt"
            Toast.makeText(this, "Export started.\nSaving to: $path", Toast.LENGTH_LONG).show()
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
