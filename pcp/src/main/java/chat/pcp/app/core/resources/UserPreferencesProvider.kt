/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.resources

import chat.pcp.app.features.settings.PcpBasePreferences
import javax.inject.Inject

class UserPreferencesProvider @Inject constructor(private val pcpPreferences: PcpBasePreferences) {

    fun shouldShowHiddenEvents() = pcpPreferences.shouldShowHiddenEvents()

    fun shouldShowReadReceipts() = pcpPreferences.showReadReceipts()

    fun shouldShowRedactedMessages() = pcpPreferences.showRedactedMessages()

    fun shouldShowLongClickOnRoomHelp() = pcpPreferences.shouldShowLongClickOnRoomHelp()

    fun neverShowLongClickOnRoomHelpAgain() {
        pcpPreferences.neverShowLongClickOnRoomHelpAgain()
    }

    fun shouldShowJoinLeaves() = pcpPreferences.showJoinLeaveMessages()

    fun shouldShowAvatarDisplayNameChanges() = pcpPreferences.showAvatarDisplayNameChangeMessages()

    fun areThreadMessagesEnabled() = pcpPreferences.areThreadMessagesEnabled()

    fun showLiveSenderInfo() = pcpPreferences.showLiveSenderInfo()

    fun autoplayAnimatedImages() = pcpPreferences.autoplayAnimatedImages()
}
