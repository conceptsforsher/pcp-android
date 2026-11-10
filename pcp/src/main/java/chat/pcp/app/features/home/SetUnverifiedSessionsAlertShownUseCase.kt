/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.lib.core.utils.timer.Clock
import javax.inject.Inject

class SetUnverifiedSessionsAlertShownUseCase @Inject constructor(
        private val pcpPreferences: PcpBasePreferences,
        private val clock: Clock,
) {

    fun execute(deviceIds: List<String>) {
        val epochMillis = clock.epochMillis()
        deviceIds.forEach {
            pcpPreferences.setUnverifiedSessionsAlertLastShownMillis(it, epochMillis)
        }
    }
}
