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

    fun shouldShowHiddenEvents(): Boolean {
        return pcpPreferences.shouldShowHiddenEvents()
    }

    fun shouldShowReadReceipts(): Boolean {
        return pcpPreferences.showReadReceipts()
    }

    fun shouldShowRedactedMessages(): Boolean {
        return pcpPreferences.showRedactedMessages()
    }

    fun shouldShowLongClickOnRoomHelp(): Boolean {
        return pcpPreferences.shouldShowLongClickOnRoomHelp()
    }

    fun neverShowLongClickOnRoomHelpAgain() {
        pcpPreferences.neverShowLongClickOnRoomHelpAgain()
    }

    fun shouldShowJoinLeaves(): Boolean {
        return pcpPreferences.showJoinLeaveMessages()
    }

    fun shouldShowAvatarDisplayNameChanges(): Boolean {
        return pcpPreferences.showAvatarDisplayNameChangeMessages()
    }

    fun areThreadMessagesEnabled(): Boolean {
        return pcpPreferences.areThreadMessagesEnabled()
    }

    fun showLiveSenderInfo(): Boolean {
        return pcpPreferences.showLiveSenderInfo()
    }

    fun autoplayAnimatedImages(): Boolean {
        return pcpPreferences.autoplayAnimatedImages()
    }
}
