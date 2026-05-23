/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.onboarding.ftueauth

import android.graphics.Color
import android.os.Bundle
import android.text.Html
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.core.view.isVisible
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.R
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

    private var currentCategory = "open"
    private var selectedProtocol = "matrix"

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentFtueAuthSplashBinding {
        return FragmentFtueAuthSplashBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        if (savedInstanceState != null) {
            currentCategory = savedInstanceState.getString(STATE_CATEGORY, "open")
            selectedProtocol = savedInstanceState.getString(STATE_PROTOCOL, "matrix")
        }
        setupViews()
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        outState.putString(STATE_CATEGORY, currentCategory)
        outState.putString(STATE_PROTOCOL, selectedProtocol)
    }

    private fun setupViews() {
        views.chipOpenSource.debouncedClicks { selectCategory("open") }
        views.chipProprietary.debouncedClicks { selectCategory("proprietary") }
        views.cardMatrix.debouncedClicks { selectProtocol("matrix") }
        views.cardIrc.debouncedClicks { selectProtocol("irc") }

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
            views.loginSplashVersion.debouncedClicks { navigator.openDebug(requireContext()) }
        }

        selectCategory(currentCategory)
    }

    private fun selectCategory(category: String) {
        currentCategory = category
        val isOpen = category == "open"

        val primaryColor = getColorFromAttr(com.google.android.material.R.attr.colorPrimary, Color.parseColor("#368BD6"))
        val quaternaryColor = getColorFromAttr(im.vector.lib.ui.styles.R.attr.vctr_content_quaternary, Color.GRAY)

        views.chipOpenSource.apply {
            setBackgroundResource(if (isOpen) R.drawable.bg_protocol_chip_selected else R.drawable.bg_protocol_chip)
            setTextColor(if (isOpen) Color.WHITE else getColorFromAttr(android.R.attr.textColorPrimary, Color.BLACK))
        }
        views.chipProprietary.apply {
            setBackgroundResource(if (isOpen) R.drawable.bg_protocol_chip else R.drawable.bg_protocol_chip_selected)
            setTextColor(if (isOpen) getColorFromAttr(android.R.attr.textColorPrimary, Color.BLACK) else Color.WHITE)
        }

        views.groupDescription.setText(
            if (isOpen) im.vector.lib.strings.R.string.open_source_group_desc
            else im.vector.lib.strings.R.string.protocol_telegram_desc
        )

        views.cardMatrix.visibility = if (isOpen) View.VISIBLE else View.INVISIBLE
        views.cardIrc.visibility = if (isOpen) View.VISIBLE else View.INVISIBLE
        views.cardXmpp.visibility = if (isOpen) View.VISIBLE else View.INVISIBLE
        views.cardDelta.visibility = if (isOpen) View.VISIBLE else View.INVISIBLE
        views.cardTelegram.visibility = if (isOpen) View.INVISIBLE else View.VISIBLE

        if (!isOpen) selectedProtocol = "telegram"
        else if (selectedProtocol == "telegram") selectedProtocol = "matrix"

        updateCardSelection()
    }

    private fun selectProtocol(protocol: String) {
        if (currentCategory == "open" && (protocol == "xmpp" || protocol == "delta")) {
            Toast.makeText(requireContext(), getString(im.vector.lib.strings.R.string.coming_soon), Toast.LENGTH_SHORT).show()
            return
        }
        selectedProtocol = protocol
        updateCardSelection()
    }

    private fun updateCardSelection() {
        val primaryColor = getColorFromAttr(com.google.android.material.R.attr.colorPrimary, Color.parseColor("#368BD6"))
        val quaternaryColor = getColorFromAttr(im.vector.lib.ui.styles.R.attr.vctr_content_quaternary, Color.GRAY)

        val isMatrix = selectedProtocol == "matrix"
        val isTelegram = selectedProtocol == "telegram"

        views.cardMatrix.apply {
            if (visibility == View.VISIBLE) {
                isChecked = isMatrix
                strokeWidth = if (isMatrix) 2 else 1
                setStrokeColor(if (isMatrix) primaryColor else quaternaryColor)
            }
        }
        views.cardMatrixIcon.setColorFilter(if (isMatrix) primaryColor else quaternaryColor)
        views.radioMatrix.isChecked = isMatrix

        views.cardIrc.apply {
            if (visibility == View.VISIBLE) {
                isChecked = !isMatrix && !isTelegram
                strokeWidth = if (!isMatrix && !isTelegram) 2 else 1
                setStrokeColor(if (!isMatrix && !isTelegram) primaryColor else quaternaryColor)
            }
        }
        views.cardIrcIcon.setColorFilter(if (!isMatrix && !isTelegram) primaryColor else quaternaryColor)
        views.radioIrc.isChecked = !isMatrix && !isTelegram

        // Features
        if (isMatrix) {
            views.loginSplashText1.setText(im.vector.lib.strings.R.string.login_splash_text1)
            views.loginSplashText2.setText(im.vector.lib.strings.R.string.login_splash_text2)
            views.loginSplashText3.setText(im.vector.lib.strings.R.string.login_splash_text3)
            views.loginSplashPicto1.setImageResource(R.drawable.ic_login_splash_message_circle)
            views.loginSplashPicto2.setImageResource(R.drawable.ic_login_splash_lock)
            views.loginSplashPicto3.setImageResource(R.drawable.ic_login_splash_sliders)
        } else if (isTelegram) {
            views.loginSplashText1.setText(im.vector.lib.strings.R.string.proprietary_feature_1)
            views.loginSplashText2.setText(im.vector.lib.strings.R.string.proprietary_feature_2)
            views.loginSplashText3.text = Html.fromHtml(getString(im.vector.lib.strings.R.string.proprietary_feature_3), Html.FROM_HTML_MODE_COMPACT)
            views.loginSplashPicto1.setImageResource(R.drawable.ic_login_splash_message_circle)
            views.loginSplashPicto2.setImageResource(R.drawable.ic_public_room)
            views.loginSplashPicto3.setImageResource(R.drawable.ic_shield_warning)
        } else {
            views.loginSplashText1.setText(im.vector.lib.strings.R.string.irc_splash_text1)
            views.loginSplashText2.setText(im.vector.lib.strings.R.string.irc_splash_text2)
            views.loginSplashText3.setText(im.vector.lib.strings.R.string.irc_splash_text3)
            views.loginSplashPicto1.setImageResource(R.drawable.ic_login_splash_message_circle)
            views.loginSplashPicto2.setImageResource(R.drawable.ic_share)
            views.loginSplashPicto3.setImageResource(R.drawable.ic_login_splash_sliders)
        }

        val isAlreadyHaveAccountEnabled = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
        views.loginSplashSubmit.text =
            if (isMatrix && isAlreadyHaveAccountEnabled) getString(CommonStrings.login_splash_create_account)
            else getString(CommonStrings.login_splash_submit)
        views.loginSplashAlreadyHaveAccount.visibility =
            if (isMatrix && isAlreadyHaveAccountEnabled) View.VISIBLE else View.INVISIBLE
    }

    private fun splashSubmit(isAlreadyHaveAccountEnabled: Boolean) {
        val getStartedFlow = if (isAlreadyHaveAccountEnabled) OnboardingFlow.SignUp else OnboardingFlow.SignInSignUp
        viewModel.handle(OnboardingAction.SplashAction.OnProtocolSelected(protocol = selectedProtocol, onboardingFlow = getStartedFlow))
    }

    private fun alreadyHaveAnAccount() {
        viewModel.handle(OnboardingAction.SplashAction.OnProtocolSelected(protocol = selectedProtocol, onboardingFlow = OnboardingFlow.SignIn))
    }

    override fun resetViewModel() {}

    private fun getColorFromAttr(attr: Int, fallback: Int): Int {
        val ta = requireContext().theme.obtainStyledAttributes(intArrayOf(attr))
        return try { ta.getColor(0, fallback) } finally { ta.recycle() }
    }

    companion object {
        private const val STATE_CATEGORY = "STATE_CATEGORY"
        private const val STATE_PROTOCOL = "STATE_PROTOCOL"
    }
}
