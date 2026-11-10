/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding.ftueauth

import android.graphics.Color
import android.graphics.drawable.Drawable
import android.graphics.drawable.LayerDrawable
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.annotation.ColorRes
import androidx.annotation.DrawableRes
import androidx.annotation.StringRes
import androidx.core.content.ContextCompat
import androidx.core.view.isGone
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.extensions.getResTintedDrawable
import chat.pcp.app.core.extensions.getTintedDrawable
import chat.pcp.app.core.extensions.setLeftDrawable
import chat.pcp.app.core.extensions.setTextWithColoredPart
import chat.pcp.app.databinding.FragmentFtueAuthUseCaseBinding
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.login.ServerType
import chat.pcp.app.features.onboarding.FtueUseCase
import chat.pcp.app.features.onboarding.OnboardingAction
import chat.pcp.app.features.themes.ThemeProvider
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

private const val DARK_MODE_ICON_BACKGROUND_ALPHA = 0.30f
private const val LIGHT_MODE_ICON_BACKGROUND_ALPHA = 0.15f

@AndroidEntryPoint
class FtueAuthUseCaseFragment :
        AbstractFtueAuthFragment<FragmentFtueAuthUseCaseBinding>() {

    @Inject lateinit var themeProvider: ThemeProvider
    @Inject lateinit var vectorFeatures: PcpFeatures

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentFtueAuthUseCaseBinding {
        return FragmentFtueAuthUseCaseBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupViews()
    }

    private fun setupViews() {
        // Connect to server relies on https://github.com/element-hq/element-android/issues/5782
        views.useCaseConnectToServerGroup.isGone = vectorFeatures.isOnboardingCombinedRegisterEnabled()

        views.useCaseOptionOne.renderUseCase(
                useCase = FtueUseCase.FRIENDS_FAMILY,
                label = CommonStrings.ftue_auth_use_case_option_one,
                icon = R.drawable.ic_use_case_friends,
                tint = chat.pcp.lib.ui.styles.R.color.palette_grape
        )
        views.useCaseOptionTwo.renderUseCase(
                useCase = FtueUseCase.TEAMS,
                label = CommonStrings.ftue_auth_use_case_option_two,
                icon = R.drawable.ic_use_case_teams,
                tint = chat.pcp.lib.ui.styles.R.color.palette_element_green
        )
        views.useCaseOptionThree.renderUseCase(
                useCase = FtueUseCase.COMMUNITIES,
                label = CommonStrings.ftue_auth_use_case_option_three,
                icon = R.drawable.ic_use_case_communities,
                tint = chat.pcp.lib.ui.styles.R.color.palette_azure
        )

        views.useCaseSkip.setTextWithColoredPart(
                fullTextRes = CommonStrings.ftue_auth_use_case_skip,
                coloredTextRes = CommonStrings.ftue_auth_use_case_skip_partial,
                underline = false,
                colorAttribute = com.google.android.material.R.attr.colorAccent,
                onClick = { viewModel.handle(OnboardingAction.UpdateUseCase(FtueUseCase.SKIP)) }
        )

        views.useCaseConnectToServer.setOnClickListener {
            viewModel.handle(OnboardingAction.UpdateServerType(ServerType.Other))
        }
    }

    override fun resetViewModel() {
        viewModel.handle(OnboardingAction.ResetUseCase)
    }

    private fun TextView.renderUseCase(useCase: FtueUseCase, @StringRes label: Int, @DrawableRes icon: Int, @ColorRes tint: Int) {
        setLeftDrawable(createIcon(tint, icon, isLightMode = themeProvider.isLightTheme()))
        setText(label)
        debouncedClicks {
            viewModel.handle(OnboardingAction.UpdateUseCase(useCase))
        }
    }

    private fun createIcon(@ColorRes tint: Int, icon: Int, isLightMode: Boolean): Drawable {
        val context = requireContext()
        val alpha = when (isLightMode) {
            true -> LIGHT_MODE_ICON_BACKGROUND_ALPHA
            false -> DARK_MODE_ICON_BACKGROUND_ALPHA
        }
        val iconBackground = context.getResTintedDrawable(R.drawable.bg_feature_icon, tint, alpha = alpha)
        val whiteLayer = context.getTintedDrawable(R.drawable.bg_feature_icon, Color.WHITE)
        return LayerDrawable(arrayOf(whiteLayer, iconBackground, ContextCompat.getDrawable(context, icon)))
    }
}
