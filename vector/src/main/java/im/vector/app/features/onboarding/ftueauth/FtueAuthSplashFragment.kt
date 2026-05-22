/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.onboarding.ftueauth

import android.annotation.SuppressLint
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.core.resources.BuildMeta
import im.vector.app.databinding.FragmentFtueAuthSplashBinding
import im.vector.app.features.VectorFeatures
import im.vector.app.features.onboarding.OnboardingAction
import im.vector.app.features.onboarding.OnboardingFlow
import im.vector.app.features.settings.VectorPreferences
import im.vector.lib.strings.CommonStrings
import javax.inject.Inject

/**
 * Modified splash: compact protocol selector (Matrix / IRC chips) above the submit button.
 */
@AndroidEntryPoint
class FtueAuthSplashFragment :
        AbstractFtueAuthFragment<FragmentFtueAuthSplashBinding>() {

    @Inject lateinit var vectorPreferences: VectorPreferences
    @Inject lateinit var vectorFeatures: VectorFeatures
    @Inject lateinit var buildMeta: BuildMeta

    private var selectedProtocol = "matrix"

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentFtueAuthSplashBinding {
        return FragmentFtueAuthSplashBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        if (savedInstanceState != null) {
            selectedProtocol = savedInstanceState.getString(STATE_PROTOCOL, "matrix")
        }
        setupViews()
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        outState.putString(STATE_PROTOCOL, selectedProtocol)
    }

    private fun setupViews() {
        views.protocolChipMatrix.debouncedClicks { onProtocolSelected("matrix") }
        views.protocolChipIrc.debouncedClicks { onProtocolSelected("irc") }

        onProtocolSelected(selectedProtocol)

        val isAlreadyHaveAccountEnabled = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
        views.loginSplashSubmit.apply {
            setText(if (isAlreadyHaveAccountEnabled) CommonStrings.login_splash_create_account else CommonStrings.login_splash_submit)
            debouncedClicks { splashSubmit(isAlreadyHaveAccountEnabled) }
        }
        views.loginSplashAlreadyHaveAccount.apply {
            isVisible = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
            debouncedClicks { alreadyHaveAnAccount() }
        }

        if (buildMeta.isDebug || vectorPreferences.developerMode()) {
            views.loginSplashVersion.isVisible = true
            @SuppressLint("SetTextI18n")
            views.loginSplashVersion.text = "Version : ${buildMeta.versionName}\n" +
                    "Branch: ${buildMeta.gitBranchName} ${buildMeta.gitRevision}"
            views.loginSplashVersion.debouncedClicks { navigator.openDebug(requireContext()) }
        }
    }

    private fun onProtocolSelected(protocol: String) {
        selectedProtocol = protocol
        val isMatrix = protocol == "matrix"

        views.protocolChipMatrix.setBackgroundResource(
            if (isMatrix) im.vector.app.R.drawable.bg_protocol_chip_selected
            else im.vector.app.R.drawable.bg_protocol_chip
        )
        views.protocolChipMatrix.setTextColor(
            if (isMatrix) android.graphics.Color.WHITE
            else requireContext().getColor(android.R.attr.textColorPrimary)
        )

        views.protocolChipIrc.setBackgroundResource(
            if (isMatrix) im.vector.app.R.drawable.bg_protocol_chip
            else im.vector.app.R.drawable.bg_protocol_chip_selected
        )
        views.protocolChipIrc.setTextColor(
            if (isMatrix) requireContext().getColor(android.R.attr.textColorPrimary)
            else android.graphics.Color.WHITE
        )

        val isAlreadyHaveAccountEnabled = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
        views.loginSplashSubmit.text =
            if (isMatrix && isAlreadyHaveAccountEnabled) requireContext().getString(CommonStrings.login_splash_create_account)
            else requireContext().getString(CommonStrings.login_splash_submit)
        views.loginSplashAlreadyHaveAccount.isVisible = isMatrix && isAlreadyHaveAccountEnabled
    }

    private fun splashSubmit(isAlreadyHaveAccountEnabled: Boolean) {
        val getStartedFlow = if (isAlreadyHaveAccountEnabled) OnboardingFlow.SignUp else OnboardingFlow.SignInSignUp
        viewModel.handle(OnboardingAction.SplashAction.OnProtocolSelected(protocol = selectedProtocol, onboardingFlow = getStartedFlow))
    }

    private fun alreadyHaveAnAccount() {
        viewModel.handle(OnboardingAction.SplashAction.OnProtocolSelected(protocol = selectedProtocol, onboardingFlow = OnboardingFlow.SignIn))
    }

    override fun resetViewModel() {
        // Nothing to do
    }

    companion object {
        private const val STATE_PROTOCOL = "STATE_PROTOCOL"
    }
}
