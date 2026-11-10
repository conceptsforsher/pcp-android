/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.di

import dagger.hilt.EntryPoint
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import chat.pcp.app.core.dialogs.UnrecognizedCertificateDialog
import chat.pcp.app.core.error.ErrorFormatter
import chat.pcp.app.features.analytics.AnalyticsTracker
import chat.pcp.app.features.call.webrtc.WebRtcCallManager
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.app.features.navigation.Navigator
import chat.pcp.app.features.pin.PinLocker
import chat.pcp.app.features.rageshake.BugReporter
import chat.pcp.app.features.session.SessionListener
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.ui.UiStateRepository
import chat.pcp.lib.core.utils.timer.Clock
import kotlinx.coroutines.CoroutineScope

@InstallIn(SingletonComponent::class)
@EntryPoint
interface SingletonEntryPoint {

    fun sessionListener(): SessionListener

    fun avatarRenderer(): AvatarRenderer

    fun activeSessionHolder(): ActiveSessionHolder

    fun unrecognizedCertificateDialog(): UnrecognizedCertificateDialog

    fun navigator(): Navigator

    fun clock(): Clock

    fun errorFormatter(): ErrorFormatter

    fun bugReporter(): BugReporter

    fun pcpPreferences(): PcpBasePreferences

    fun uiStateRepository(): UiStateRepository

    fun pinLocker(): PinLocker

    fun analyticsTracker(): AnalyticsTracker

    fun webRtcCallManager(): WebRtcCallManager

    fun appCoroutineScope(): CoroutineScope
}
