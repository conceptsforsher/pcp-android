package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.core.view.isVisible
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramProfileBinding

class TelegramProxyActivity : VectorBaseActivity<ActivityTelegramProfileBinding>() {

    companion object {
        fun newIntent(context: Context): Intent =
            Intent(context, TelegramProxyActivity::class.java)
    }

    override fun getBinding() = ActivityTelegramProfileBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.apply {
            title = "Proxy Settings"
            setDisplayHomeAsUpEnabled(true)
        }

        views.tgProfileEditBtn.isVisible = false
        views.tgProfileLogoutBtn.isVisible = false
        views.tgProfileName.text = "Proxy Configuration"
        views.tgProfileUsername.text = "Add SOCKS5, HTTP or MTProto proxy"
        views.tgProfileBio.text = ""

        views.tgProfileSharedBtn.setOnClickListener {
            showAddProxyDialog()
        }
        (views.tgProfileSharedBtn.getChildAt(0) as? android.widget.TextView)?.text = "Add Proxy"

        views.tgProfileGroupsBtn.setOnClickListener {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetProxies(h)
            Toast.makeText(this, "Loading proxies...", Toast.LENGTH_SHORT).show()
        }
        (views.tgProfileGroupsBtn.getChildAt(0) as? android.widget.TextView)?.text = "List Proxies"

        views.tgProfileSessionsBtn.text = "Disable Proxy"
        views.tgProfileSessionsBtn.setOnClickListener {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgDisableProxy(h)
            Toast.makeText(this, "Proxy disabled", Toast.LENGTH_SHORT).show()
        }
    }

    private fun showAddProxyDialog() {
        val layout = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
        }
        val serverInput = EditText(this).apply { hint = "Server (IP or host)" }
        val portInput = EditText(this).apply { hint = "Port"; inputType = android.text.InputType.TYPE_CLASS_NUMBER }
        val typeInput = EditText(this).apply { hint = "Type: socks5, http, mtproto"; setText("socks5") }
        val secretInput = EditText(this).apply { hint = "Secret (MTProto only)" }

        layout.addView(serverInput)
        layout.addView(portInput)
        layout.addView(typeInput)
        layout.addView(secretInput)

        AlertDialog.Builder(this)
            .setTitle("Add Proxy")
            .setView(layout)
            .setPositiveButton("Add") { _, _ ->
                val server = serverInput.text.toString().trim()
                val port = portInput.text.toString().toIntOrNull() ?: 1080
                val type = typeInput.text.toString().trim().lowercase()
                val secret = secretInput.text.toString().trim()

                if (server.isNotBlank()) {
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgAddProxy(
                        h, server, port, true, type,
                        "", "", secret
                    )
                    Toast.makeText(this, "Proxy added: $server:$port", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }
}
