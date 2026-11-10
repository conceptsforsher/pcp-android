package chat.pcp.app.features.auth

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import androidx.core.view.isVisible
import androidx.lifecycle.lifecycleScope
import chat.pcp.app.irc.IrcClient
import chat.pcp.app.irc.IrcNative
import chat.pcp.app.irc.IrcServerInfo
import chat.pcp.app.irc.IrcSessionHolder
import im.vector.app.R as AppR
import im.vector.lib.strings.R as StringsR
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityIrcAuthBinding
import im.vector.app.features.home.HomeActivity
import dagger.hilt.android.AndroidEntryPoint
import kotlinx.coroutines.flow.collect
import kotlinx.coroutines.launch
import javax.inject.Inject

@AndroidEntryPoint
class IrcAuthActivity : VectorBaseActivity<ActivityIrcAuthBinding>() {

    @Inject lateinit var ircSessionHolder: IrcSessionHolder

    override val rootView: View
        get() = views.root

    override fun getBinding() = ActivityIrcAuthBinding.inflate(layoutInflater)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        if (!IrcNative.ensureLoaded()) {
            views.ircAuthStatus.text = getString(StringsR.string.irc_connection_error)
            views.ircAuthStatus.isVisible = true
            views.ircAuthButton.isEnabled = false
            return
        }

        views.ircAuthButton.debouncedClicks { connect() }
    }

    private fun connect() {
        val host = views.ircServerHost.text.toString().trim()
        val portStr = views.ircServerPort.text.toString().trim()
        val nick = views.ircNickname.text.toString().trim()
        val password = views.ircPassword.text.toString().trim()
        val tls = views.ircTlsSwitch.isChecked

        if (host.isEmpty() || nick.isEmpty()) {
            views.ircAuthStatus.text = "Host and nickname are required"
            views.ircAuthStatus.isVisible = true
            return
        }

        val port = portStr.toIntOrNull() ?: if (tls) 6697 else 6667

        views.ircAuthButton.isEnabled = false
        views.ircAuthStatus.text = getString(StringsR.string.irc_connecting, host)
        views.ircAuthStatus.isVisible = true

        ircSessionHolder.getOrCreateSession() ?: run {
            views.ircAuthStatus.text = getString(StringsR.string.irc_connection_error)
            views.ircAuthStatus.isVisible = true
            views.ircAuthButton.isEnabled = true
            return
        }

        val ircClient = ircSessionHolder.getIrcClient() ?: run {
            views.ircAuthStatus.text = getString(StringsR.string.irc_connection_error)
            views.ircAuthStatus.isVisible = true
            views.ircAuthButton.isEnabled = true
            return
        }

        ircClient.connect(
            IrcServerInfo(
                host = host,
                port = port,
                tls = tls,
                nick = nick,
                user = nick,
                realName = nick,
                password = password
            )
        )

        lifecycleScope.launch {
            ircClient.state.collect { state ->
                when (state) {
                    IrcClient.State.REGISTERED -> {
                        views.ircAuthStatus.text = getString(StringsR.string.irc_connected, nick)
                        views.ircAuthStatus.isVisible = true
                        val intent = HomeActivity.newIntent(
                            this@IrcAuthActivity,
                            firstStartMainActivity = true,
                            authenticationDescription = null
                        )
                        startActivity(intent)
                        finish()
                    }
                    IrcClient.State.ERROR -> {
                        views.ircAuthStatus.text = getString(StringsR.string.irc_connection_error)
                        views.ircAuthStatus.isVisible = true
                        views.ircAuthButton.isEnabled = true
                    }
                    else -> {}
                }
            }
        }
    }

    companion object {
        fun newIntent(context: Context): Intent {
            return Intent(context, IrcAuthActivity::class.java)
        }
    }
}
