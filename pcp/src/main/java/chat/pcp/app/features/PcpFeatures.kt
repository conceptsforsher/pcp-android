/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features

import chat.pcp.app.config.Config
import chat.pcp.app.config.OnboardingVariant
import chat.pcp.app.features.settings.PcpBasePreferences
import javax.inject.Inject

interface PcpFeatures {

    fun onboardingVariant(): OnboardingVariant
    fun isOnboardingAlreadyHaveAccountSplashEnabled(): Boolean
    fun isOnboardingSplashCarouselEnabled(): Boolean
    fun isOnboardingUseCaseEnabled(): Boolean
    fun isOnboardingPersonalizeEnabled(): Boolean
    fun isOnboardingCombinedRegisterEnabled(): Boolean
    fun isOnboardingCombinedLoginEnabled(): Boolean
    fun allowExternalUnifiedPushDistributors(): Boolean
    fun isScreenSharingEnabled(): Boolean
    fun isLocationSharingEnabled(): Boolean
    fun forceUsageOfOpusEncoder(): Boolean

    /**
     * This is only to enable if the labs flag should be visible and effective.
     * If on the client-side you want functionality that should be enabled with the new layout,
     * use [PcpBasePreferences.isNewAppLayoutEnabled] instead.
     */
    fun isNewAppLayoutFeatureEnabled(): Boolean
    fun isVoiceBroadcastEnabled(): Boolean
    fun isUnverifiedSessionsAlertEnabled(): Boolean
}

class DefaultPcpFeatures @Inject constructor() : PcpFeatures {
    override fun onboardingVariant() = Config.ONBOARDING_VARIANT
    override fun isOnboardingAlreadyHaveAccountSplashEnabled() = true
    override fun isOnboardingSplashCarouselEnabled() = true
    override fun isOnboardingUseCaseEnabled() = true
    override fun isOnboardingPersonalizeEnabled() = true
    override fun isOnboardingCombinedRegisterEnabled() = true
    override fun isOnboardingCombinedLoginEnabled() = true
    override fun allowExternalUnifiedPushDistributors(): Boolean = Config.ALLOW_EXTERNAL_UNIFIED_PUSH_DISTRIBUTORS
    override fun isScreenSharingEnabled(): Boolean = true
    override fun isLocationSharingEnabled() = Config.ENABLE_LOCATION_SHARING
    override fun forceUsageOfOpusEncoder(): Boolean = false
    override fun isNewAppLayoutFeatureEnabled(): Boolean = true
    override fun isVoiceBroadcastEnabled(): Boolean = true
    override fun isUnverifiedSessionsAlertEnabled(): Boolean = true
}
