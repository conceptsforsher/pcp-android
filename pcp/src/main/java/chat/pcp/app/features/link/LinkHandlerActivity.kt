/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.link

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.view.View
import androidx.lifecycle.lifecycleScope
import com.airbnb.mvrx.viewModel
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.registerStartForActivityResult
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.core.utils.toast
import chat.pcp.app.databinding.ActivityProgressBinding
import chat.pcp.app.features.MainActivity
import chat.pcp.app.features.home.HomeActivity
import chat.pcp.app.features.login.LoginConfig
import chat.pcp.app.features.permalink.PermalinkHandler
import chat.pcp.app.features.start.StartAppViewModel
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.session.permalinks.PermalinkService
import timber.log.Timber
import javax.inject.Inject

/**
 * Dummy activity used to dispatch the vector URL links.
 */
@AndroidEntryPoint
class LinkHandlerActivity : PcpActivity<ActivityProgressBinding>() {

    @Inject lateinit var permalinkHandler: PermalinkHandler

    private val startAppViewModel: StartAppViewModel by viewModel()

    override fun getBinding() = ActivityProgressBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.mainRoot

    override fun initUiAndData() {
        handleIntent()
    }

    private val launcher = registerStartForActivityResult {
        if (it.resultCode == RESULT_OK) {
            handleIntent()
        } else {
            // User has pressed back on the MainActivity, so finish also this one.
            finish()
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        if (startAppViewModel.shouldStartApp()) {
            launcher.launch(MainActivity.getIntentToInitSession(this))
        } else {
            handleIntent()
        }
    }

    override fun onNewIntent(intent: Intent) {
        super.onNewIntent(intent)
        handleIntent()
    }

    private fun handleIntent() {
        val uri = intent.data
        when {
            uri == null -> {
                // Should not happen
                Timber.w("Uri is null")
                finish()
            }
            uri.getQueryParameter(LoginConfig.CONFIG_HS_PARAMETER) != null -> handleConfigUrl(uri)
            uri.toString().startsWith(PermalinkService.MATRIX_TO_URL_BASE) -> handleSupportedHostUrl()
            uri.toString().startsWith(PermalinkHandler.MATRIX_TO_CUSTOM_SCHEME_URL_BASE) -> handleSupportedHostUrl()
            resources.getStringArray(chat.pcp.app.config.R.array.permalink_supported_hosts).contains(uri.host) -> handleSupportedHostUrl()
            else -> {
                // Other links are not yet handled, but should not come here (manifest configuration error?)
                toast(CommonStrings.universal_link_malformed)
                finish()
            }
        }
    }

    private fun handleConfigUrl(uri: Uri) {
        if (activeSessionHolder.hasActiveSession()) {
            displayAlreadyLoginPopup(uri)
        } else {
            // user is not yet logged in, this is the nominal case
            startLoginActivity(uri)
        }
    }

    private fun handleSupportedHostUrl() {
        // If we are not logged in, open login screen.
        // In the future, we might want to relaunch the process after login.
        if (!activeSessionHolder.hasActiveSession()) {
            startLoginActivity()
            return
        }

        // We forward intent to HomeActivity (singleTask) to avoid the dueling app problem
        // https://stackoverflow.com/questions/25884954/deep-linking-and-multiple-app-instances
        intent.setClass(this, HomeActivity::class.java)
        intent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP or Intent.FLAG_ACTIVITY_SINGLE_TOP
        startActivity(intent)
    }

    /**
     * Start the login screen with identity server and homeserver pre-filled, if any.
     */
    private fun startLoginActivity(uri: Uri? = null) {
        navigator.openLogin(
                context = this,
                loginConfig = uri?.let { LoginConfig.parse(uri) },
                flags = Intent.FLAG_ACTIVITY_CLEAR_TOP or Intent.FLAG_ACTIVITY_CLEAR_TASK or Intent.FLAG_ACTIVITY_NEW_TASK
        )
        finish()
    }

    /**
     * Propose to disconnect from a previous HS, when clicking on an auto config link.
     */
    private fun displayAlreadyLoginPopup(uri: Uri) {
        MaterialAlertDialogBuilder(this)
                .setTitle(CommonStrings.dialog_title_warning)
                .setMessage(CommonStrings.error_user_already_logged_in)
                .setCancelable(false)
                .setPositiveButton(CommonStrings.logout) { _, _ -> safeSignout(uri) }
                .setNegativeButton(CommonStrings.action_cancel) { _, _ -> finish() }
                .show()
    }

    private fun safeSignout(uri: Uri) {
        val session = activeSessionHolder.getSafeActiveSession()
        if (session == null) {
            // Should not happen
            startLoginActivity(uri)
        } else {
            lifecycleScope.launch {
                try {
                    session.signOutService().signOut(true)
                    Timber.d("## displayAlreadyLoginPopup(): logout succeeded")
                    activeSessionHolder.clearActiveSession()
                    startLoginActivity(uri)
                } catch (failure: Throwable) {
                    displayError(failure)
                }
            }
        }
    }

    private fun displayError(failure: Throwable) {
        MaterialAlertDialogBuilder(this)
                .setTitle(CommonStrings.dialog_title_error)
                .setMessage(errorFormatter.toHumanReadable(failure))
                .setCancelable(false)
                .setPositiveButton(CommonStrings.ok) { _, _ -> finish() }
                .show()
    }
}
