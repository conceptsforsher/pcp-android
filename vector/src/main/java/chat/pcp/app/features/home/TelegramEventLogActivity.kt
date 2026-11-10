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

class TelegramEventLogActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        private const val EXTRA_CHAT_ID = "chat_id"
        fun newIntent(context: Context, chatId: Long): Intent =
            Intent(context, TelegramEventLogActivity::class.java).putExtra(EXTRA_CHAT_ID, chatId)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val chatId = intent.getLongExtra(EXTRA_CHAT_ID, 0)
        supportActionBar?.apply {
            title = "Event Log"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        val label = android.widget.TextView(this).apply {
            text = "Recent Admin Actions"
            textSize = 18f
            setTextColor(0xFF000000.toInt())
            setPadding(0, 0, 0, 16)
        }
        container.addView(label)

        val events = listOf(
            EventItem("12:34", "Admin", "changed group title to \"New Name\""),
            EventItem("12:30", "Admin", "pinned a message"),
            EventItem("12:25", "User123", "joined the group via invite link"),
            EventItem("12:20", "User456", "was removed by Admin"),
            EventItem("12:15", "Admin", "changed Slow Mode to 30 seconds"),
            EventItem("12:10", "Bot", "was promoted to administrator"),
            EventItem("12:05", "Admin", "changed group description"),
            EventItem("12:00", "Admin", "created invite link")
        )

        events.forEach { event ->
            val row = android.widget.LinearLayout(this).apply {
                orientation = android.widget.LinearLayout.HORIZONTAL
                setPadding(0, 8, 0, 8)
            }
            val timeView = android.widget.TextView(this).apply {
                text = event.time
                textSize = 12f
                setTextColor(0xFF888888.toInt())
                layoutParams = android.widget.LinearLayout.LayoutParams(
                    android.widget.LinearLayout.LayoutParams.WRAP_CONTENT,
                    android.widget.LinearLayout.LayoutParams.WRAP_CONTENT
                ).apply { marginEnd = 12 }
            }
            val contentView = android.widget.TextView(this).apply {
                text = "${event.actor}: ${event.action}"
                textSize = 14f
                setTextColor(0xFF000000.toInt())
            }
            row.addView(timeView)
            row.addView(contentView)
            container.addView(row)
        }

        addButton(container, "Load Administrators") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatAdministrators(h, chatId)
            Toast.makeText(this, "Loading admins...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Load Recent Actions") {
            Toast.makeText(this, "Loading recent admin actions...", Toast.LENGTH_SHORT).show()
        }

        val scrollView = android.widget.ScrollView(this).apply { addView(container) }
        views.telegramMsgRecycler.visibility = android.view.View.GONE
        val root = findViewById<android.view.ViewGroup>(android.R.id.content)
        root.addView(scrollView, android.view.ViewGroup.LayoutParams(
            android.view.ViewGroup.LayoutParams.MATCH_PARENT,
            android.view.ViewGroup.LayoutParams.MATCH_PARENT
        ))
    }

    data class EventItem(val time: String, val actor: String, val action: String)

    private fun addButton(container: android.widget.LinearLayout, text: String, onClick: () -> Unit) {
        val btn = android.widget.Button(this).apply {
            this.text = text
            setTextColor(0xFF000000.toInt())
            setOnClickListener { onClick() }
        }
        container.addView(btn)
    }
}
