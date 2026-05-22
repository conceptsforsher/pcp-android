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
import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.RadioButton
import android.widget.TextView
import android.widget.Toast
import androidx.core.view.isVisible
import com.google.android.material.card.MaterialCardView
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

        selectCategory(currentCategory)
    }

    // ── Category selection ────────────────────────────────────────────

    private fun selectCategory(category: String) {
        currentCategory = category
        val isOpen = category == "open"

        val primaryColor = getColorFromAttr(com.google.android.material.R.attr.colorPrimary, Color.parseColor("#368BD6"))

        views.chipOpenSource.apply {
            setBackgroundResource(if (isOpen) R.drawable.bg_protocol_chip_selected else R.drawable.bg_protocol_chip)
            setTextColor(if (isOpen) Color.WHITE else requireContext().getColor(android.R.attr.textColorPrimary))
        }
        views.chipProprietary.apply {
            setBackgroundResource(if (isOpen) R.drawable.bg_protocol_chip else R.drawable.bg_protocol_chip_selected)
            setTextColor(if (isOpen) requireContext().getColor(android.R.attr.textColorPrimary) else Color.WHITE)
        }

        views.groupDescription.setText(
            if (isOpen) im.vector.lib.strings.R.string.open_source_group_desc
            else im.vector.lib.strings.R.string.protocol_telegram_desc
        )

        if (isOpen) {
            if (selectedProtocol == "telegram") selectedProtocol = "matrix"
        } else {
            selectedProtocol = "telegram"
        }

        buildProtocolCards()
    }

    // ── Protocol cards ────────────────────────────────────────────────

    private fun buildProtocolCards() {
        val container = views.protocolCardsContainer
        container.removeAllViews()

        val primaryColor = getColorFromAttr(com.google.android.material.R.attr.colorPrimary, Color.parseColor("#368BD6"))
        val quaternaryColor = getColorFromAttr(im.vector.lib.ui.styles.R.attr.vctr_content_quaternary, Color.GRAY)
        val textPrimary = getColorFromAttr(android.R.attr.textColorPrimary, Color.BLACK)
        val textSecondary = getColorFromAttr(android.R.attr.textColorSecondary, Color.DKGRAY)
        val cardBackground = getColorFromAttr(android.R.attr.colorBackground, Color.WHITE)

        val protocols = when (currentCategory) {
            "open" -> listOf(
                ProtocolInfo("matrix", im.vector.lib.strings.R.string.protocol_matrix, im.vector.lib.strings.R.string.protocol_matrix_desc, R.drawable.ic_chat, false),
                ProtocolInfo("irc", im.vector.lib.strings.R.string.protocol_irc, im.vector.lib.strings.R.string.protocol_irc_desc, R.drawable.ic_login_splash_message_circle, false),
                ProtocolInfo("xmpp", im.vector.lib.strings.R.string.protocol_xmpp, im.vector.lib.strings.R.string.protocol_xmpp_desc, R.drawable.ic_chat, true),
                ProtocolInfo("delta", im.vector.lib.strings.R.string.protocol_delta, im.vector.lib.strings.R.string.protocol_delta_desc, R.drawable.ic_login_splash_message_circle, true),
            )
            else -> listOf(
                ProtocolInfo("telegram", im.vector.lib.strings.R.string.protocol_telegram, im.vector.lib.strings.R.string.protocol_telegram_desc, R.drawable.ic_chat, true),
            )
        }

        for (proto in protocols) {
            val card = MaterialCardView(requireContext()).apply {
                layoutParams = LinearLayout.LayoutParams(
                    LinearLayout.LayoutParams.MATCH_PARENT,
                    LinearLayout.LayoutParams.WRAP_CONTENT
                ).apply { bottomMargin = 6.dpToPx() }
                setRadius(10.dpToPx().toFloat())
                setCardElevation(1.dpToPx().toFloat())
                setCardBackgroundColor(cardBackground)
                isClickable = !proto.isStub
                isFocusable = !proto.isStub
                isChecked = selectedProtocol == proto.id

                val alpha = if (proto.isStub) 0.4f else 1.0f
                strokeWidth = if (selectedProtocol == proto.id) 2 else 1
                setStrokeColor(if (selectedProtocol == proto.id) primaryColor else quaternaryColor)

                val inner = LinearLayout(requireContext()).apply {
                    orientation = LinearLayout.HORIZONTAL
                    gravity = Gravity.CENTER_VERTICAL
                    setPadding(10.dpToPx(), 10.dpToPx(), 10.dpToPx(), 10.dpToPx())
                    setAlpha(if (proto.isStub) 0.4f else 1.0f)
                }

                val icon = ImageView(requireContext()).apply {
                    layoutParams = LinearLayout.LayoutParams(28.dpToPx(), 28.dpToPx())
                    setImageResource(proto.iconRes)
                    setColorFilter(if (selectedProtocol == proto.id) primaryColor else quaternaryColor)
                }
                inner.addView(icon)

                val textCol = LinearLayout(requireContext()).apply {
                    orientation = LinearLayout.VERTICAL
                    layoutParams = LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT, 1f).apply {
                        marginStart = 8.dpToPx()
                    }
                }

                val title = TextView(requireContext()).apply {
                    text = getString(proto.titleRes)
                    setTextColor(textPrimary)
                    textSize = 13f
                    setTypeface(typeface, android.graphics.Typeface.BOLD)
                }
                textCol.addView(title)

                val desc = TextView(requireContext()).apply {
                    text = if (proto.isStub) getString(im.vector.lib.strings.R.string.coming_soon)
                           else getString(proto.descRes)
                    setTextColor(textSecondary)
                    textSize = 11f
                }
                textCol.addView(desc)
                inner.addView(textCol)

                val radio = RadioButton(requireContext()).apply {
                    isChecked = selectedProtocol == proto.id
                    isClickable = false
                    isFocusable = false
                    setAlpha(if (proto.isStub) 0.4f else 1.0f)
                }
                inner.addView(radio)

                addView(inner)

                setOnClickListener {
                    if (proto.isStub) {
                        Toast.makeText(requireContext(), getString(im.vector.lib.strings.R.string.coming_soon), Toast.LENGTH_SHORT).show()
                    } else {
                        selectProtocol(proto.id)
                    }
                }
            }
            container.addView(card)
        }

        onProtocolSelected(selectedProtocol)
    }

    // ── Protocol selection ────────────────────────────────────────────

    private fun selectProtocol(protocol: String) {
        selectedProtocol = protocol
        buildProtocolCards()
    }

    private data class ProtocolInfo(
        val id: String,
        val titleRes: Int,
        val descRes: Int,
        val iconRes: Int,
        val isStub: Boolean
    )

    private fun onProtocolSelected(protocol: String) {
        val isMatrix = protocol == "matrix"

        if (isMatrix) {
            views.loginSplashText1.setText(im.vector.lib.strings.R.string.login_splash_text1)
            views.loginSplashText2.setText(im.vector.lib.strings.R.string.login_splash_text2)
            views.loginSplashText3.setText(im.vector.lib.strings.R.string.login_splash_text3)
            views.loginSplashPicto1.setImageResource(R.drawable.ic_login_splash_message_circle)
            views.loginSplashPicto2.setImageResource(R.drawable.ic_login_splash_lock)
            views.loginSplashPicto3.setImageResource(R.drawable.ic_login_splash_sliders)
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

    // ── Actions ───────────────────────────────────────────────────────

    private fun splashSubmit(isAlreadyHaveAccountEnabled: Boolean) {
        val getStartedFlow = if (isAlreadyHaveAccountEnabled) OnboardingFlow.SignUp else OnboardingFlow.SignInSignUp
        viewModel.handle(OnboardingAction.SplashAction.OnProtocolSelected(protocol = selectedProtocol, onboardingFlow = getStartedFlow))
    }

    private fun alreadyHaveAnAccount() {
        viewModel.handle(OnboardingAction.SplashAction.OnProtocolSelected(protocol = selectedProtocol, onboardingFlow = OnboardingFlow.SignIn))
    }

    override fun resetViewModel() {
        // nothing
    }

    // ── Helpers ───────────────────────────────────────────────────────

    private fun getColorFromAttr(attr: Int, fallback: Int): Int {
        val ta = requireContext().theme.obtainStyledAttributes(intArrayOf(attr))
        return try { ta.getColor(0, fallback) } finally { ta.recycle() }
    }

    private fun Int.dpToPx(): Int = (this * resources.displayMetrics.density).toInt()

    companion object {
        private const val STATE_CATEGORY = "STATE_CATEGORY"
        private const val STATE_PROTOCOL = "STATE_PROTOCOL"
    }
}
