/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2

import chat.pcp.app.features.settings.PcpBasePreferences
import javax.inject.Inject

class ToggleIpAddressVisibilityUseCase @Inject constructor(
        private val pcpPreferences: PcpBasePreferences,
) {

    fun execute() {
        val currentVisibility = pcpPreferences.showIpAddressInSessionManagerScreens()
        pcpPreferences.setIpAddressVisibilityInDeviceManagerScreens(!currentVisibility)
    }
}
