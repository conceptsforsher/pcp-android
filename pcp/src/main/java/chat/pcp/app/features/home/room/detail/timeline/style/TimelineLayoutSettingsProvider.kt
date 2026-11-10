/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.style

import chat.pcp.app.features.settings.PcpBasePreferences
import javax.inject.Inject

class TimelineLayoutSettingsProvider @Inject constructor(private val pcpPreferences: PcpBasePreferences) {

    fun getLayoutSettings(): TimelineLayoutSettings {
        return if (pcpPreferences.useMessageBubblesLayout()) {
            TimelineLayoutSettings.BUBBLE
        } else {
            TimelineLayoutSettings.MODERN
        }
    }
}
