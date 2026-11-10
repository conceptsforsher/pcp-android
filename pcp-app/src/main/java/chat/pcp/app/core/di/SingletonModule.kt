/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.di

import android.app.Application
import android.content.Context
import android.content.Context.MODE_PRIVATE
import android.content.SharedPreferences
import android.content.res.Resources
import androidx.preference.PreferenceManager
import com.google.i18n.phonenumbers.PhoneNumberUtil
import dagger.Binds
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import chat.pcp.app.EmojiCompatWrapper
import chat.pcp.app.EmojiSpanify
import chat.pcp.app.SpaceStateHandler
import chat.pcp.app.SpaceStateHandlerImpl
import chat.pcp.app.config.Config
import chat.pcp.app.core.device.DefaultGetDeviceInfoUseCase
import chat.pcp.app.core.device.GetDeviceInfoUseCase
import chat.pcp.app.core.dispatchers.CoroutineDispatchers
import chat.pcp.app.core.error.DefaultErrorFormatter
import chat.pcp.app.core.error.ErrorFormatter
import chat.pcp.app.core.resources.BuildMeta
import chat.pcp.app.core.utils.AndroidSystemSettingsProvider
import chat.pcp.app.core.utils.SystemSettingsProvider
import chat.pcp.app.features.analytics.AnalyticsTracker
import chat.pcp.app.features.analytics.PcpAnalytics
import chat.pcp.app.features.analytics.errors.ErrorTracker
import chat.pcp.app.features.analytics.impl.DefaultPcpAnalytics
import chat.pcp.app.features.analytics.metrics.PcpPlugins
import chat.pcp.app.features.configuration.PcpEventTypes
import chat.pcp.app.features.invite.AutoAcceptInvites
import chat.pcp.app.features.invite.CompileTimeAutoAcceptInvites
import chat.pcp.app.features.mdm.DefaultMdmService
import chat.pcp.app.features.mdm.MdmData
import chat.pcp.app.features.mdm.MdmService
import chat.pcp.app.features.navigation.DefaultNavigator
import chat.pcp.app.features.navigation.Navigator
import chat.pcp.app.features.pin.PinCodeStore
import chat.pcp.app.features.pin.SharedPrefPinCodeStore
import chat.pcp.app.features.room.PcpRoomDisplayName
import chat.pcp.app.features.settings.FontScalePreferences
import chat.pcp.app.features.settings.FontScalePreferencesImpl
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.ui.SharedPreferencesUiStateRepository
import chat.pcp.app.features.ui.UiStateRepository
import chat.pcp.application.BuildConfig
import chat.pcp.application.R
import chat.pcp.lib.core.utils.timer.Clock
import chat.pcp.lib.core.utils.timer.DefaultClock
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.SupervisorJob
import org.matrix.android.sdk.api.Matrix
import org.matrix.android.sdk.api.MatrixConfiguration
import org.matrix.android.sdk.api.SyncConfig
import org.matrix.android.sdk.api.auth.AuthenticationService
import org.matrix.android.sdk.api.auth.HomeServerHistoryService
import org.matrix.android.sdk.api.raw.RawService
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.sync.filter.SyncFilterParams
import org.matrix.android.sdk.api.settings.LightweightSettingsStorage
import javax.inject.Singleton

@InstallIn(SingletonComponent::class) @Module abstract class PcpBindModule {

    @Binds
    abstract fun bindNavigator(navigator: DefaultNavigator): Navigator

    @Binds
    abstract fun bindVectorAnalytics(analytics: DefaultPcpAnalytics): PcpAnalytics

    @Binds
    abstract fun bindErrorTracker(analytics: DefaultPcpAnalytics): ErrorTracker

    @Binds
    abstract fun bindAnalyticsTracker(analytics: DefaultPcpAnalytics): AnalyticsTracker

    @Binds
    abstract fun bindErrorFormatter(formatter: DefaultErrorFormatter): ErrorFormatter

    @Binds
    abstract fun bindUiStateRepository(repository: SharedPreferencesUiStateRepository): UiStateRepository

    @Binds
    abstract fun bindPinCodeStore(store: SharedPrefPinCodeStore): PinCodeStore

    @Binds
    abstract fun bindAutoAcceptInvites(autoAcceptInvites: CompileTimeAutoAcceptInvites): AutoAcceptInvites

    @Binds
    abstract fun bindEmojiSpanify(emojiCompatWrapper: EmojiCompatWrapper): EmojiSpanify

    @Binds
    abstract fun bindMdmService(service: DefaultMdmService): MdmService

    @Binds
    abstract fun bindFontScale(fontScale: FontScalePreferencesImpl): FontScalePreferences

    @Binds
    abstract fun bindSystemSettingsProvide(provider: AndroidSystemSettingsProvider): SystemSettingsProvider

    @Binds
    abstract fun bindSpaceStateHandler(spaceStateHandlerImpl: SpaceStateHandlerImpl): SpaceStateHandler

    @Binds
    abstract fun bindGetDeviceInfoUseCase(getDeviceInfoUseCase: DefaultGetDeviceInfoUseCase): GetDeviceInfoUseCase
}

@InstallIn(SingletonComponent::class) @Module object PcpStaticModule {

    @Provides
    fun providesContext(application: Application): Context {
        return application.applicationContext
    }

    @Provides
    fun providesResources(context: Context): Resources {
        return context.resources
    }

    @Provides
    fun providesSharedPreferences(context: Context): SharedPreferences {
        return context.getSharedPreferences("chat.pcp.riot", MODE_PRIVATE)
    }

    @Provides
    fun providesMatrixConfiguration(
            vectorPreferences: PcpBasePreferences,
            pcpRoomDisplayName: PcpRoomDisplayName,
            vectorPlugins: PcpPlugins,
            pcpEventTypes: PcpEventTypes,
            mdmService: MdmService,
    ): MatrixConfiguration {
        return MatrixConfiguration(
                applicationFlavor = BuildConfig.FLAVOR_DESCRIPTION,
                roomDisplayNameFallbackProvider = pcpRoomDisplayName,
                threadMessagesEnabledDefault = vectorPreferences.areThreadMessagesEnabled(),
                networkInterceptors = emptyList(),
                metricPlugins = vectorPlugins.plugins(),
                cryptoAnalyticsPlugin = vectorPlugins.cryptoMetricPlugin,
                customEventTypesProvider = pcpEventTypes,
                clientPermalinkBaseUrl = mdmService.getData(MdmData.PermalinkBaseUrl),
                syncConfig = SyncConfig(
                        syncFilterParams = SyncFilterParams(lazyLoadMembersForStateEvents = true, useThreadNotifications = true)
                )
        )
    }

    @Provides
    @Singleton
    fun providesMatrix(context: Context, configuration: MatrixConfiguration): Matrix {
        return Matrix(context, configuration)
    }

    @Provides
    fun providesCurrentSession(activeSessionHolder: ActiveSessionHolder): Session {
        // TODO handle session injection better
        return activeSessionHolder.getActiveSession()
    }

    @Provides
    fun providesAuthenticationService(matrix: Matrix): AuthenticationService {
        return matrix.authenticationService()
    }

    @Provides
    fun providesRawService(matrix: Matrix): RawService {
        return matrix.rawService()
    }

    @Provides
    fun providesLightweightSettingsStorage(matrix: Matrix): LightweightSettingsStorage {
        return matrix.lightweightSettingsStorage()
    }

    @Provides
    fun providesHomeServerHistoryService(matrix: Matrix): HomeServerHistoryService {
        return matrix.homeServerHistoryService()
    }

    @Provides
    @Singleton
    fun providesApplicationCoroutineScope() = CoroutineScope(SupervisorJob() + Dispatchers.Main)

    @Provides
    fun providesCoroutineDispatchers() = CoroutineDispatchers(io = Dispatchers.IO, computation = Dispatchers.Default)

    @OptIn(DelicateCoroutinesApi::class)
    @Provides
    @NamedGlobalScope
    fun providesGlobalScope() = GlobalScope

    @Provides
    fun providesPhoneNumberUtil(): PhoneNumberUtil = PhoneNumberUtil.getInstance()

    @Provides
    @Singleton
    fun providesBuildMeta(context: Context) = BuildMeta(
            isDebug = BuildConfig.DEBUG,
            applicationId = BuildConfig.APPLICATION_ID,
            applicationName = context.getString(R.string.app_name),
            lowPrivacyLoggingEnabled = Config.LOW_PRIVACY_LOG_ENABLE,
            versionName = BuildConfig.VERSION_NAME,
            gitRevision = BuildConfig.GIT_REVISION,
            gitRevisionDate = BuildConfig.GIT_REVISION_DATE,
            gitBranchName = BuildConfig.GIT_BRANCH_NAME,
            flavorDescription = BuildConfig.FLAVOR_DESCRIPTION,
            flavorShortDescription = BuildConfig.SHORT_FLAVOR_DESCRIPTION,
    )

    @Provides
    @Singleton
    @DefaultPreferences
    fun providesDefaultSharedPreferences(context: Context): SharedPreferences {
        return PreferenceManager.getDefaultSharedPreferences(context.applicationContext)
    }

    @Singleton
    @Provides
    fun providesDefaultClock(): Clock = DefaultClock()
}
