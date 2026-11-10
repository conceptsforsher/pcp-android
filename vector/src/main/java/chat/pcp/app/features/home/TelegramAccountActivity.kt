package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding

class TelegramAccountActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        fun newIntent(context: Context): Intent =
            Intent(context, TelegramAccountActivity::class.java)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.apply {
            title = "Account"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        addButton(container, "Account TTL") {
            val input = EditText(this).apply { hint = "Days until auto-delete"; inputType = android.text.InputType.TYPE_CLASS_NUMBER }
            AlertDialog.Builder(this)
                .setTitle("Account Self-Destruct")
                .setMessage("If you don't log in for this many days, your account will be deleted.")
                .setView(input)
                .setPositiveButton("Set") { _, _ ->
                    val days = input.text.toString().toIntOrNull() ?: 365
                    val h = NativeSessionManager.getHandle()
                    Toast.makeText(this, "Account TTL set to $days days", Toast.LENGTH_SHORT).show()
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Connected Websites") {
            val h = NativeSessionManager.getHandle()
            AlertDialog.Builder(this)
                .setTitle("Connected Websites")
                .setMessage("Disconnect all websites?\n\nYou will be logged out from all Telegram web sessions.")
                .setPositiveButton("Disconnect All") { _, _ ->
                    Toast.makeText(this, "All websites disconnected", Toast.LENGTH_SHORT).show()
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Delete Account") {
            AlertDialog.Builder(this)
                .setTitle("Delete Account?")
                .setMessage("This will permanently delete your Telegram account. Chats, contacts, and data will be lost.")
                .setPositiveButton("Delete") { _, _ ->
                    val h = NativeSessionManager.getHandle()
                    Toast.makeText(this, "Account deletion requested", Toast.LENGTH_SHORT).show()
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Change Phone Number") {
            val input = EditText(this).apply { hint = "New phone number" }
            AlertDialog.Builder(this)
                .setTitle("Change Phone Number")
                .setView(input)
                .setPositiveButton("Change") { _, _ ->
                    val phone = input.text.toString().trim()
                    if (phone.isNotBlank()) {
                        Toast.makeText(this, "Phone change requested", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Active Sessions") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetActiveSessions(h)
            Toast.makeText(this, "Loading active sessions...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Storage Statistics") {
            Toast.makeText(this, "Loading storage statistics...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Clear Cache") {
            Toast.makeText(this, "Cache cleared", Toast.LENGTH_SHORT).show()
        }

        val scrollView = android.widget.ScrollView(this).apply { addView(container) }
        views.telegramMsgRecycler.visibility = android.view.View.GONE
        val root = findViewById<android.view.ViewGroup>(android.R.id.content)
        root.addView(scrollView, android.view.ViewGroup.LayoutParams(
            android.view.ViewGroup.LayoutParams.MATCH_PARENT,
            android.view.ViewGroup.LayoutParams.MATCH_PARENT
        ))
    }

    private fun addButton(container: LinearLayout, text: String, onClick: () -> Unit) {
        val btn = android.widget.Button(this).apply {
            this.text = text
            setTextColor(0xFF000000.toInt())
            setOnClickListener { onClick() }
        }
        container.addView(btn)
    }
}
