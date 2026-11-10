/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import chat.pcp.app.config.Config
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.lib.core.utils.timer.Clock
import javax.inject.Inject

class ShouldShowUnverifiedSessionsAlertUseCase @Inject constructor(
        private val vectorFeatures: PcpFeatures,
        private val pcpPreferences: PcpBasePreferences,
        private val clock: Clock,
) {

    fun execute(deviceId: String?): Boolean {
        deviceId ?: return false

        val isUnverifiedSessionsAlertEnabled = vectorFeatures.isUnverifiedSessionsAlertEnabled()
        val unverifiedSessionsAlertLastShownMillis = pcpPreferences.getUnverifiedSessionsAlertLastShownMillis(deviceId)
        return isUnverifiedSessionsAlertEnabled &&
                clock.epochMillis() - unverifiedSessionsAlertLastShownMillis >= Config.SHOW_UNVERIFIED_SESSIONS_ALERT_AFTER_MILLIS
    }
}
