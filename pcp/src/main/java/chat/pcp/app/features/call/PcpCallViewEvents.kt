/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call

import chat.pcp.app.core.platform.PcpViewEvents
import chat.pcp.app.features.call.audio.CallAudioManager
import org.matrix.android.sdk.api.session.call.TurnServerResponse

sealed class PcpCallViewEvents : PcpViewEvents {

    data class ConnectionTimeout(val turn: TurnServerResponse?) : PcpCallViewEvents()
    data class ShowSoundDeviceChooser(
            val available: Set<CallAudioManager.Device>,
            val current: CallAudioManager.Device
    ) : PcpCallViewEvents()

    object ShowDialPad : PcpCallViewEvents()
    object ShowCallTransferScreen : PcpCallViewEvents()
    object FailToTransfer : PcpCallViewEvents()
    object ShowScreenSharingPermissionDialog : PcpCallViewEvents()
    object StopScreenSharingService : PcpCallViewEvents()
}
