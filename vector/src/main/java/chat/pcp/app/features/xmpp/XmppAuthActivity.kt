package chat.pcp.app.features.xmpp

import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import chat.pcp.app.xmpp.XmppNative
import chat.pcp.app.xmpp.XmppSessionHolder
import dagger.hilt.android.AndroidEntryPoint
import javax.inject.Inject
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

@AndroidEntryPoint
class XmppAuthActivity : AppCompatActivity() {

    @Inject
    lateinit var xmppHolder: XmppSessionHolder

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val layout = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(48, 96, 48, 48)
        }

        val title = TextView(this).apply {
            text = "XMPP Login"
            textSize = 24f
        }
        layout.addView(title)

        val protocolLabel = TextView(this).apply {
            text = "Protocol: XMPP (Jabber)"
            textSize = 14f
            setPadding(0, 8, 0, 24)
        }
        layout.addView(protocolLabel)

        val jidEdit = EditText(this).apply {
            hint = "JID (user@domain)"
        }
        layout.addView(jidEdit)

        val passEdit = EditText(this).apply {
            hint = "Password"
        }
        layout.addView(passEdit)

        val serverEdit = EditText(this).apply {
            hint = "Server (optional)"
        }
        layout.addView(serverEdit)

        val portEdit = EditText(this).apply {
            hint = "Port"
            setText("5222")
        }
        layout.addView(portEdit)

        val statusText = TextView(this).apply {
            text = ""
            textSize = 14f
            setPadding(0, 8, 0, 0)
        }
        layout.addView(statusText)

        val connectBtn = Button(this).apply {
            text = "Connect"
            setOnClickListener {
                val jid = jidEdit.text.toString().trim()
                val pass = passEdit.text.toString()
                val server = serverEdit.text.toString().trim()
                val port = portEdit.text.toString().toIntOrNull() ?: 5222

                if (jid.isBlank() || pass.isBlank()) {
                    statusText.text = "Please fill in JID and password"
                    return@setOnClickListener
                }

                statusText.text = "Connecting..."
                CoroutineScope(Dispatchers.IO).launch {
                    val session = xmppHolder.connectWithAccount(jid, pass, server, port)
                    runOnUiThread {
                        if (session != null) {
                            statusText.text = "Connected! JID: ${session.userId}"
                            Toast.makeText(this@XmppAuthActivity,
                                "XMPP connected: ${session.userId}", Toast.LENGTH_SHORT).show()
                            finish()
                        } else {
                            statusText.text = "Connection failed"
                        }
                    }
                }
            }
        }
        layout.addView(connectBtn)

        setContentView(layout)
    }
}
