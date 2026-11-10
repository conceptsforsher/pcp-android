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

class TelegramPrivacyActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        fun newIntent(context: Context): Intent =
            Intent(context, TelegramPrivacyActivity::class.java)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.apply {
            title = "Privacy & Security"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        addButton(container, "Active Sessions") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetActiveSessions(h)
            Toast.makeText(this, "Loading sessions...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Terminate All Other Sessions") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgTerminateAllOtherSessions(h)
            Toast.makeText(this, "All other sessions terminated", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Blocked Users") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetBlockedUsers(h, 0, 100)
            Toast.makeText(this, "Loading blocked users...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Close Friends") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetCloseFriends(h)
            Toast.makeText(this, "Loading close friends...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Story Privacy") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetStoryPrivacy(h)
            Toast.makeText(this, "Loading story privacy...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Notification Settings") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetNotificationSettings(h)
            Toast.makeText(this, "Loading notification settings...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Connected Websites") {
            Toast.makeText(this, "Loading connected websites...", Toast.LENGTH_SHORT).show()
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
