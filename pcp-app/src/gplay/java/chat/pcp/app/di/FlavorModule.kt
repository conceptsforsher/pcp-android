/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.di

import dagger.Binds
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import chat.pcp.app.GoogleFlavorLegals
import chat.pcp.app.core.pushers.FcmHelper
import chat.pcp.app.core.resources.AppNameProvider
import chat.pcp.app.core.resources.DefaultAppNameProvider
import chat.pcp.app.core.resources.DefaultLocaleProvider
import chat.pcp.app.core.resources.LocaleProvider
import chat.pcp.app.core.services.GuardServiceStarter
import chat.pcp.app.features.home.NightlyProxy
import chat.pcp.app.features.settings.legals.FlavorLegals
import chat.pcp.app.nightly.FirebaseNightlyProxy
import chat.pcp.app.push.fcm.GoogleFcmHelper

@InstallIn(SingletonComponent::class)
@Module
abstract class FlavorModule {

    companion object {
        @Provides
        fun provideGuardServiceStarter() = object : GuardServiceStarter {}
    }

    @Binds
    abstract fun bindsNightlyProxy(nightlyProxy: FirebaseNightlyProxy): NightlyProxy

    @Binds
    abstract fun bindsFcmHelper(fcmHelper: GoogleFcmHelper): FcmHelper

    @Binds
    abstract fun bindsLocaleProvider(localeProvider: DefaultLocaleProvider): LocaleProvider

    @Binds
    abstract fun bindsAppNameProvider(appNameProvider: DefaultAppNameProvider): AppNameProvider

    @Binds
    abstract fun bindsFlavorLegals(legals: GoogleFlavorLegals): FlavorLegals
}
