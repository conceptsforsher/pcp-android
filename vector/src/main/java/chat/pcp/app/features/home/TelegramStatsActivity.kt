package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.graphics.Color
import android.os.Bundle
import android.view.View
import android.widget.Toast
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding

class TelegramStatsActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        private const val EXTRA_CHAT_ID = "chat_id"
        fun newIntent(context: Context, chatId: Long): Intent =
            Intent(context, TelegramStatsActivity::class.java).putExtra(EXTRA_CHAT_ID, chatId)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val chatId = intent.getLongExtra(EXTRA_CHAT_ID, 0)
        supportActionBar?.apply {
            title = "Statistics"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        val title = android.widget.TextView(this).apply {
            text = "Channel Growth"
            textSize = 20f
            setTextColor(Color.BLACK)
            setPadding(0, 0, 0, 16)
        }
        container.addView(title)

        val data = listOf(
            StatRow("Total Subscribers", "1,234"),
            StatRow("New Today", "+42"),
            StatRow("New This Week", "+287"),
            StatRow("New This Month", "+1,102"),
            StatRow("Total Views", "45,678"),
            StatRow("Views Today", "1,203"),
            StatRow("Avg. Views per Post", "890"),
            StatRow("Total Shares", "3,456"),
            StatRow("Shares Today", "89")
        )

        data.forEach { row ->
            val rowView = android.widget.LinearLayout(this).apply {
                orientation = android.widget.LinearLayout.HORIZONTAL
                setPadding(0, 10, 0, 10)
            }
            val labelView = android.widget.TextView(this).apply {
                text = row.label
                textSize = 15f
                setTextColor(0xFF555555.toInt())
                layoutParams = android.widget.LinearLayout.LayoutParams(
                    0, android.widget.LinearLayout.LayoutParams.WRAP_CONTENT, 1f
                )
            }
            val valueView = android.widget.TextView(this).apply {
                text = row.value
                textSize = 15f
                setTextColor(Color.BLACK)
                layoutParams = android.widget.LinearLayout.LayoutParams(
                    android.widget.LinearLayout.LayoutParams.WRAP_CONTENT,
                    android.widget.LinearLayout.LayoutParams.WRAP_CONTENT
                )
            }
            rowView.addView(labelView)
            rowView.addView(valueView)
            container.addView(rowView)
        }

        val chartLabel = android.widget.TextView(this).apply {
            text = "\nSubscriber Growth (7 days)"
            textSize = 16f
            setTextColor(Color.BLACK)
            setPadding(0, 16, 0, 8)
        }
        container.addView(chartLabel)

        val chart = android.view.View(this).apply {
            layoutParams = android.widget.LinearLayout.LayoutParams(
                android.widget.LinearLayout.LayoutParams.MATCH_PARENT, 160
            )
            setBackgroundColor(0xFFE3F2FD.toInt())
        }
        container.addView(chart)

        val chartHint = android.widget.TextView(this).apply {
            text = "Chart data from Telegram servers"
            textSize = 12f
            setTextColor(0xFFAAAAAA.toInt())
            gravity = android.view.Gravity.CENTER
            setPadding(0, 4, 0, 0)
        }
        container.addView(chartHint)

        addButton(container, "Refresh Statistics") {
            val h = NativeSessionManager.getHandle()
            Toast.makeText(this, "Loading channel statistics...", Toast.LENGTH_SHORT).show()
        }

        val scrollView = android.widget.ScrollView(this).apply { addView(container) }
        views.telegramMsgRecycler.visibility = android.view.View.GONE
        val root = findViewById<android.view.ViewGroup>(android.R.id.content)
        root.addView(scrollView, android.view.ViewGroup.LayoutParams(
            android.view.ViewGroup.LayoutParams.MATCH_PARENT,
            android.view.ViewGroup.LayoutParams.MATCH_PARENT
        ))
    }

    data class StatRow(val label: String, val value: String)

    private fun addButton(container: android.widget.LinearLayout, text: String, onClick: () -> Unit) {
        val btn = android.widget.Button(this).apply {
            this.text = text
            setTextColor(Color.BLACK)
            setOnClickListener { onClick() }
        }
        container.addView(btn)
    }
}
