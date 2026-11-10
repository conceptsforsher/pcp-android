/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding

import chat.pcp.app.config.OnboardingVariant
import chat.pcp.app.core.platform.ScreenOrientationLocker
import chat.pcp.app.core.resources.BuildMeta
import chat.pcp.app.databinding.ActivityLoginBinding
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.onboarding.ftueauth.FtueAuthVariant
import javax.inject.Inject

class OnboardingVariantFactory @Inject constructor(
        private val vectorFeatures: PcpFeatures,
        private val orientationLocker: ScreenOrientationLocker,
        private val buildMeta: BuildMeta,
) {

    fun create(
            activity: OnboardingActivity,
            views: ActivityLoginBinding,
            onboardingViewModel: Lazy<OnboardingViewModel>,
    ) = when (vectorFeatures.onboardingVariant()) {
        OnboardingVariant.LEGACY -> error("Legacy is not supported by the FTUE")
        OnboardingVariant.FTUE_AUTH -> FtueAuthVariant(
                views = views,
                onboardingViewModel = onboardingViewModel.value,
                activity = activity,
                supportFragmentManager = activity.supportFragmentManager,
                vectorFeatures = vectorFeatures,
                orientationLocker = orientationLocker,
                buildMeta = buildMeta,
        )
    }
}
