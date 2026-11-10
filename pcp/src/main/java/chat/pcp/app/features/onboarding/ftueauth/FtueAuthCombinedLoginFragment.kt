/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding.ftueauth

import android.os.Build
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.autofill.HintConstants
import androidx.core.view.isVisible
import androidx.lifecycle.flowWithLifecycle
import androidx.lifecycle.lifecycleScope
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.clearErrorOnChange
import chat.pcp.app.core.extensions.content
import chat.pcp.app.core.extensions.editText
import chat.pcp.app.core.extensions.hideKeyboard
import chat.pcp.app.core.extensions.hidePassword
import chat.pcp.app.core.extensions.realignPercentagesToParent
import chat.pcp.app.core.extensions.setOnFocusLostListener
import chat.pcp.app.core.extensions.setOnImeDoneListener
import chat.pcp.app.core.extensions.toReducedUrl
import chat.pcp.app.databinding.FragmentFtueCombinedLoginBinding
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.login.LoginMode
import chat.pcp.app.features.login.SSORedirectRouterActivity
import chat.pcp.app.features.login.SocialLoginButtonsView
import chat.pcp.app.features.login.render
import chat.pcp.app.features.onboarding.OnboardingAction
import chat.pcp.app.features.onboarding.OnboardingViewEvents
import chat.pcp.app.features.onboarding.OnboardingViewState
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.flow.combine
import kotlinx.coroutines.flow.launchIn
import org.matrix.android.sdk.api.auth.SSOAction
import reactivecircus.flowbinding.android.widget.textChanges
import javax.inject.Inject

@AndroidEntryPoint
class FtueAuthCombinedLoginFragment :
        AbstractSSOFtueAuthFragment<FragmentFtueCombinedLoginBinding>() {

    @Inject lateinit var loginFieldsValidation: LoginFieldsValidation
    @Inject lateinit var loginErrorParser: LoginErrorParser
    @Inject lateinit var vectorFeatures: PcpFeatures

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentFtueCombinedLoginBinding {
        return FragmentFtueCombinedLoginBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupSubmitButton()
        views.loginRoot.realignPercentagesToParent()
        views.editServerButton.debouncedClicks { viewModel.handle(OnboardingAction.PostViewEvent(OnboardingViewEvents.EditServerSelection)) }
        views.loginPasswordInput.setOnImeDoneListener { submit() }
        views.loginInput.setOnFocusLostListener(viewLifecycleOwner) {
            viewModel.handle(OnboardingAction.UserNameEnteredAction.Login(views.loginInput.content()))
        }
        views.loginForgotPassword.debouncedClicks { viewModel.handle(OnboardingAction.PostViewEvent(OnboardingViewEvents.OnForgetPasswordClicked)) }
    }

    private fun setupSubmitButton() {
        views.loginSubmit.setOnClickListener { submit() }
        views.loginInput.clearErrorOnChange(viewLifecycleOwner)
        views.loginPasswordInput.clearErrorOnChange(viewLifecycleOwner)

        combine(views.loginInput.editText().textChanges(), views.loginPasswordInput.editText().textChanges()) { account, password ->
            views.loginSubmit.isEnabled = account.isNotEmpty() && password.isNotEmpty()
        }.flowWithLifecycle(lifecycle).launchIn(viewLifecycleOwner.lifecycleScope)
    }

    private fun submit() {
        cleanupUi()
        loginFieldsValidation.validate(views.loginInput.content(), views.loginPasswordInput.content())
                .onUsernameOrIdError { views.loginInput.error = it }
                .onPasswordError { views.loginPasswordInput.error = it }
                .onValid { usernameOrId, password ->
                    val initialDeviceName = getString(CommonStrings.login_default_session_public_name)
                    viewModel.handle(OnboardingAction.AuthenticateAction.Login(usernameOrId, password, initialDeviceName))
                }
    }

    private fun cleanupUi() {
        views.loginSubmit.hideKeyboard()
        views.loginInput.error = null
        views.loginPasswordInput.error = null
    }

    override fun resetViewModel() {
        viewModel.handle(OnboardingAction.ResetAuthenticationAttempt)
    }

    override fun onError(throwable: Throwable) {
        // Trick to display the error without text.
        views.loginInput.error = " "
        loginErrorParser.parse(throwable, views.loginPasswordInput.content())
                .onUnknown { super.onError(it) }
                .onUsernameOrIdError { views.loginInput.error = it }
                .onPasswordError { views.loginPasswordInput.error = it }
    }

    override fun updateWithState(state: OnboardingViewState) {
        setupUi(state)
        setupAutoFill()

        views.selectedServerName.text = state.selectedHomeserver.userFacingUrl.toReducedUrl()

        if (state.isLoading) {
            // Ensure password is hidden
            views.loginPasswordInput.editText().hidePassword()
        }
    }

    private fun setupUi(state: OnboardingViewState) {
        when (state.selectedHomeserver.preferredLoginMode) {
            is LoginMode.SsoAndPassword -> {
                showUsernamePassword()
                renderSsoProviders(state.deviceId, state.selectedHomeserver.preferredLoginMode)
            }
            is LoginMode.Sso -> {
                hideUsernamePassword()
                renderSsoProviders(state.deviceId, state.selectedHomeserver.preferredLoginMode)
            }
            else -> {
                showUsernamePassword()
                hideSsoProviders()
            }
        }
    }

    private fun renderSsoProviders(deviceId: String?, loginMode: LoginMode) {
        views.ssoGroup.isVisible = true
        views.ssoButtonsHeader.isVisible = isUsernameAndPasswordVisible()
        views.ssoButtons.render(loginMode, SocialLoginButtonsView.Mode.MODE_CONTINUE) { id ->
            viewModel.fetchSsoUrl(
                    redirectUrl = SSORedirectRouterActivity.PCP_REDIRECT_URL,
                    deviceId = deviceId,
                    provider = id,
                    action = SSOAction.LOGIN
            )?.let { openInCustomTab(it) }
        }
    }

    private fun hideSsoProviders() {
        views.ssoGroup.isVisible = false
        views.ssoButtons.ssoIdentityProviders = null
    }

    private fun hideUsernamePassword() {
        views.loginEntryGroup.isVisible = false
    }

    private fun showUsernamePassword() {
        views.loginEntryGroup.isVisible = true
    }

    private fun isUsernameAndPasswordVisible() = views.loginEntryGroup.isVisible

    private fun setupAutoFill() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            views.loginInput.setAutofillHints(HintConstants.AUTOFILL_HINT_NEW_USERNAME)
            views.loginPasswordInput.setAutofillHints(HintConstants.AUTOFILL_HINT_NEW_PASSWORD)
        }
    }
}
