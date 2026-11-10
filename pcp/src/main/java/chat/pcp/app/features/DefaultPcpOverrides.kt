/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features

import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flowOf

interface PcpOverrides {
    val forceDialPad: Flow<Boolean>
    val forceLoginFallback: Flow<Boolean>
    val forceHomeserverCapabilities: Flow<HomeserverCapabilitiesOverride>?
}

data class HomeserverCapabilitiesOverride(
        val canChangeDisplayName: Boolean?,
        val canChangeAvatar: Boolean?
)

class DefaultPcpOverrides : PcpOverrides {
    override val forceDialPad = flowOf(false)
    override val forceLoginFallback = flowOf(false)
    override val forceHomeserverCapabilities: Flow<HomeserverCapabilitiesOverride>? = null
}
