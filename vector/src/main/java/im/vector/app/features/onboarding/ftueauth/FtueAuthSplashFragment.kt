/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.onboarding.ftueauth

import android.annotation.SuppressLint
import android.graphics.Color
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
        views.cardMatrix.debouncedClicks { onProtocolSelected("matrix") }
        views.cardIrc.debouncedClicks { onProtocolSelected("irc") }

        val isAlreadyHaveAccountEnabled = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
        views.loginSplashSubmit.apply {
            setText(if (isAlreadyHaveAccountEnabled) CommonStrings.login_splash_create_account else CommonStrings.login_splash_submit)
            debouncedClicks { splashSubmit(isAlreadyHaveAccountEnabled) }
        }
        views.loginSplashAlreadyHaveAccount.apply {
            visibility = if (vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()) View.VISIBLE else View.INVISIBLE
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

        val primaryColor = getColorFromAttr(com.google.android.material.R.attr.colorPrimary, Color.parseColor("#368BD6"))
        val quaternaryColor = getColorFromAttr(im.vector.lib.ui.styles.R.attr.vctr_content_quaternary, Color.GRAY)

        // Matrix card
        views.cardMatrix.apply {
            isChecked = isMatrix
            strokeWidth = if (isMatrix) 2 else 1
            setStrokeColor(if (isMatrix) primaryColor else quaternaryColor)
        }
        views.cardMatrixIcon.setColorFilter(if (isMatrix) primaryColor else quaternaryColor)
        views.radioMatrix.isChecked = isMatrix

        // IRC card
        views.cardIrc.apply {
            isChecked = !isMatrix
            strokeWidth = if (isMatrix) 1 else 2
            setStrokeColor(if (isMatrix) quaternaryColor else primaryColor)
        }
        views.cardIrcIcon.setColorFilter(if (isMatrix) quaternaryColor else primaryColor)
        views.radioIrc.isChecked = !isMatrix

        // Swap feature texts and icons per protocol
        if (isMatrix) {
            views.loginSplashText1.text = getString(im.vector.lib.strings.R.string.login_splash_text1)
            views.loginSplashText2.text = getString(im.vector.lib.strings.R.string.login_splash_text2)
            views.loginSplashText3.text = getString(im.vector.lib.strings.R.string.login_splash_text3)
            views.loginSplashPicto1.setImageResource(im.vector.app.R.drawable.ic_login_splash_message_circle)
            views.loginSplashPicto2.setImageResource(im.vector.app.R.drawable.ic_login_splash_lock)
            views.loginSplashPicto3.setImageResource(im.vector.app.R.drawable.ic_login_splash_sliders)
        } else {
            views.loginSplashText1.text = getString(im.vector.lib.strings.R.string.irc_splash_text1)
            views.loginSplashText2.text = getString(im.vector.lib.strings.R.string.irc_splash_text2)
            views.loginSplashText3.text = getString(im.vector.lib.strings.R.string.irc_splash_text3)
            views.loginSplashPicto1.setImageResource(im.vector.app.R.drawable.ic_login_splash_message_circle)
            views.loginSplashPicto2.setImageResource(im.vector.app.R.drawable.ic_share)
            views.loginSplashPicto3.setImageResource(im.vector.app.R.drawable.ic_login_splash_sliders)
        }

        val isAlreadyHaveAccountEnabled = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
        views.loginSplashSubmit.text =
            if (isMatrix && isAlreadyHaveAccountEnabled) getString(CommonStrings.login_splash_create_account)
            else getString(CommonStrings.login_splash_submit)
        views.loginSplashAlreadyHaveAccount.visibility = if (isMatrix && isAlreadyHaveAccountEnabled) View.VISIBLE else View.INVISIBLE
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

    private fun getColorFromAttr(attr: Int, fallback: Int): Int {
        val ta = requireContext().theme.obtainStyledAttributes(intArrayOf(attr))
        val color = try { ta.getColor(0, fallback) } finally { ta.recycle() }
        return color
    }

    companion object {
        private const val STATE_PROTOCOL = "STATE_PROTOCOL"
    }
}
