package chat.pcp.app.features.xmpp

import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import chat.pcp.app.xmpp.XmppSessionHolder
import dagger.hilt.android.AndroidEntryPoint
import javax.inject.Inject

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
            hint = "Port (default 5222)"
            setText("5222")
        }
        layout.addView(portEdit)

        val statusText = TextView(this).apply {
            text = ""
            textSize = 14f
        }
        layout.addView(statusText)

        val connectBtn = Button(this).apply {
            text = "Connect"
            setOnClickListener {
                val jid = jidEdit.text.toString()
                val pass = passEdit.text.toString()
                val server = serverEdit.text.toString()
                val port = portEdit.text.toString().toIntOrNull() ?: 5222

                if (jid.isBlank() || pass.isBlank()) {
                    statusText.text = "Please fill in JID and password"
                    return@setOnClickListener
                }

                statusText.text = "Connecting..."
                val session = xmppHolder.connectWithAccount(jid, pass, server, port)
                if (session != null) {
                    statusText.text = "Connected! JID: ${session.userId}"
                } else {
                    statusText.text = "Connection failed"
                }
            }
        }
        layout.addView(connectBtn)

        setContentView(layout)
    }
}
