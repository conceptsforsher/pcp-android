/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import chat.pcp.app.features.settings.PcpBasePreferences
import javax.inject.Inject

class IsNewLoginAlertShownUseCase @Inject constructor(
        private val pcpPreferences: PcpBasePreferences,
) {

    fun execute(deviceId: String?): Boolean {
        deviceId ?: return false

        return pcpPreferences.isNewLoginAlertShownForDevice(deviceId)
    }
}
