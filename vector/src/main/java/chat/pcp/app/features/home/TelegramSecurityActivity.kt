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

class TelegramSecurityActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        fun newIntent(context: Context): Intent =
            Intent(context, TelegramSecurityActivity::class.java)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.apply {
            title = "Security"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        addButton(container, "Set Password (2FA)") {
            val layout = android.widget.LinearLayout(this).apply {
                orientation = android.widget.LinearLayout.VERTICAL
            }
            val oldPw = EditText(this).apply {
                hint = "Current password"
                inputType = android.text.InputType.TYPE_TEXT_VARIATION_PASSWORD
            }
            val newPw = EditText(this).apply {
                hint = "New password"
                inputType = android.text.InputType.TYPE_TEXT_VARIATION_PASSWORD
            }
            val hint = EditText(this).apply { hint = "Password hint" }
            layout.addView(oldPw); layout.addView(newPw); layout.addView(hint)

            AlertDialog.Builder(this)
                .setTitle("Two-Factor Authentication")
                .setView(layout)
                .setPositiveButton("Set") { _, _ ->
                    val h = NativeSessionManager.getHandle()
                    val newPwText = newPw.text.toString()
                    if (newPwText.isNotBlank()) {
                        PcpNative.tgSendPassword(h, oldPw.text.toString())
                        Toast.makeText(this, "Password change requested", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Recovery Email") {
            val input = EditText(this).apply {
                hint = "Recovery email address"
                inputType = android.text.InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS
            }
            AlertDialog.Builder(this)
                .setTitle("Recovery Email")
                .setView(input)
                .setPositiveButton("Set") { _, _ ->
                    val email = input.text.toString().trim()
                    if (email.isNotBlank()) {
                        Toast.makeText(this, "Recovery email set: $email", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Check Username") {
            val input = EditText(this).apply { hint = "Username to check" }
            AlertDialog.Builder(this)
                .setTitle("Check Username Availability")
                .setView(input)
                .setPositiveButton("Check") { _, _ ->
                    val username = input.text.toString().trim()
                    if (username.isNotBlank()) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgSetUsername(h, username)
                        Toast.makeText(this, "Checking username: @$username", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Set Username") {
            val input = EditText(this).apply { hint = "New username (without @)" }
            AlertDialog.Builder(this)
                .setTitle("Set Username")
                .setView(input)
                .setPositiveButton("Set") { _, _ ->
                    val username = input.text.toString().trim()
                    if (username.isNotBlank()) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgSetUsername(h, username)
                        Toast.makeText(this, "Username set: @$username", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Get Premium State") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetPremiumState(h)
            Toast.makeText(this, "Loading premium state...", Toast.LENGTH_SHORT).show()
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
