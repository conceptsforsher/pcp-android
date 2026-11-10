/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding.ftueauth

import android.annotation.SuppressLint
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.resources.BuildMeta
import chat.pcp.app.databinding.FragmentFtueAuthSplashBinding
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.onboarding.OnboardingAction
import chat.pcp.app.features.onboarding.OnboardingFlow
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

/**
 * In this screen, the user is viewing an introduction to what he can do with this application.
 */
@AndroidEntryPoint
class FtueAuthSplashFragment :
        AbstractFtueAuthFragment<FragmentFtueAuthSplashBinding>() {

    @Inject lateinit var pcpPreferences: PcpBasePreferences
    @Inject lateinit var vectorFeatures: PcpFeatures
    @Inject lateinit var buildMeta: BuildMeta

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentFtueAuthSplashBinding {
        return FragmentFtueAuthSplashBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupViews()
    }

    private fun setupViews() {
        val isAlreadyHaveAccountEnabled = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
        views.loginSplashSubmit.apply {
            setText(if (isAlreadyHaveAccountEnabled) CommonStrings.login_splash_create_account else CommonStrings.login_splash_submit)
            debouncedClicks { splashSubmit(isAlreadyHaveAccountEnabled) }
        }
        views.loginSplashAlreadyHaveAccount.apply {
            isVisible = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
            debouncedClicks { alreadyHaveAnAccount() }
        }

        if (buildMeta.isDebug || pcpPreferences.developerMode()) {
            views.loginSplashVersion.isVisible = true
            @SuppressLint("SetTextI18n")
            views.loginSplashVersion.text = "Version : ${buildMeta.versionName}\n" +
                    "Branch: ${buildMeta.gitBranchName} ${buildMeta.gitRevision}"
            views.loginSplashVersion.debouncedClicks { navigator.openDebug(requireContext()) }
        }
    }

    private fun splashSubmit(isAlreadyHaveAccountEnabled: Boolean) {
        val getStartedFlow = if (isAlreadyHaveAccountEnabled) OnboardingFlow.SignUp else OnboardingFlow.SignInSignUp
        viewModel.handle(OnboardingAction.SplashAction.OnGetStarted(onboardingFlow = getStartedFlow))
    }

    private fun alreadyHaveAnAccount() {
        viewModel.handle(OnboardingAction.SplashAction.OnIAlreadyHaveAnAccount(onboardingFlow = OnboardingFlow.SignIn))
    }

    override fun resetViewModel() {
        // Nothing to do
    }
}
