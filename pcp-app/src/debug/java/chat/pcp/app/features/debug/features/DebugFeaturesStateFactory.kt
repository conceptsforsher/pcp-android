/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.debug.features

import androidx.datastore.preferences.core.Preferences
import chat.pcp.app.features.DefaultPcpFeatures
import chat.pcp.app.features.PcpFeatures
import javax.inject.Inject
import kotlin.reflect.KFunction1

class DebugFeaturesStateFactory @Inject constructor(
        private val debugFeatures: DebugPcpFeatures,
        private val defaultFeatures: DefaultPcpFeatures
) {

    fun create(): FeaturesState {
        return FeaturesState(
                listOf(
                        createEnumFeature(
                                label = "Onboarding variant",
                                featureOverride = debugFeatures.onboardingVariant(),
                                featureDefault = defaultFeatures.onboardingVariant()
                        ),
                        createBooleanFeature(
                                label = "FTUE Splash - I already have an account",
                                key = DebugFeatureKeys.onboardingAlreadyHaveAnAccount,
                                factory = PcpFeatures::isOnboardingAlreadyHaveAccountSplashEnabled
                        ),
                        createBooleanFeature(
                                label = "FTUE Splash - carousel",
                                key = DebugFeatureKeys.onboardingSplashCarousel,
                                factory = PcpFeatures::isOnboardingSplashCarouselEnabled
                        ),
                        createBooleanFeature(
                                label = "FTUE Use Case",
                                key = DebugFeatureKeys.onboardingUseCase,
                                factory = PcpFeatures::isOnboardingUseCaseEnabled
                        ),
                        createBooleanFeature(
                                label = "FTUE Personalize profile",
                                key = DebugFeatureKeys.onboardingPersonalize,
                                factory = PcpFeatures::isOnboardingPersonalizeEnabled
                        ),
                        createBooleanFeature(
                                label = "FTUE Combined register",
                                key = DebugFeatureKeys.onboardingCombinedRegister,
                                factory = PcpFeatures::isOnboardingCombinedRegisterEnabled
                        ),
                        createBooleanFeature(
                                label = "FTUE Combined login",
                                key = DebugFeatureKeys.onboardingCombinedLogin,
                                factory = PcpFeatures::isOnboardingCombinedLoginEnabled
                        ),
                        createBooleanFeature(
                                label = "Allow external UnifiedPush distributors",
                                key = DebugFeatureKeys.allowExternalUnifiedPushDistributors,
                                factory = PcpFeatures::allowExternalUnifiedPushDistributors
                        ),
                        createBooleanFeature(
                                label = "Enable Live Location Sharing",
                                key = DebugFeatureKeys.liveLocationSharing,
                                factory = PcpFeatures::isLocationSharingEnabled
                        ),
                        createBooleanFeature(
                                label = "Force usage of OpusEncoder library",
                                key = DebugFeatureKeys.forceUsageOfOpusEncoder,
                                factory = PcpFeatures::forceUsageOfOpusEncoder
                        ),
                        createBooleanFeature(
                                label = "Enable New App Layout",
                                key = DebugFeatureKeys.newAppLayoutEnabled,
                                factory = PcpFeatures::isNewAppLayoutFeatureEnabled
                        ),
                        createBooleanFeature(
                                label = "Enable Voice Broadcast",
                                key = DebugFeatureKeys.voiceBroadcastEnabled,
                                factory = PcpFeatures::isVoiceBroadcastEnabled
                        ),
                )
        )
    }

    private fun createBooleanFeature(key: Preferences.Key<Boolean>, label: String, factory: KFunction1<PcpFeatures, Boolean>): Feature {
        return Feature.BooleanFeature(
                label = label,
                featureOverride = factory.invoke(debugFeatures).takeIf { debugFeatures.hasOverride(key) },
                featureDefault = factory.invoke(defaultFeatures),
                key = key
        )
    }

    private inline fun <reified T : Enum<T>> createEnumFeature(label: String, featureOverride: T, featureDefault: T): Feature {
        return Feature.EnumFeature(
                label = label,
                override = featureOverride.takeIf { debugFeatures.hasEnumOverride(T::class) },
                default = featureDefault,
                options = enumValues<T>().toList(),
                type = T::class
        )
    }
}
