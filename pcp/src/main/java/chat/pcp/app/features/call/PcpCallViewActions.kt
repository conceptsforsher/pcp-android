/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call

import chat.pcp.app.core.platform.PcpViewModelAction
import chat.pcp.app.features.call.audio.CallAudioManager
import chat.pcp.app.features.call.transfer.CallTransferResult
import org.webrtc.VideoCapturer

sealed class PcpCallViewActions : PcpViewModelAction {
    object EndCall : PcpCallViewActions()
    object AcceptCall : PcpCallViewActions()
    object DeclineCall : PcpCallViewActions()
    object ToggleMute : PcpCallViewActions()
    object ToggleVideo : PcpCallViewActions()
    object ToggleHoldResume : PcpCallViewActions()
    data class ChangeAudioDevice(val device: CallAudioManager.Device) : PcpCallViewActions()
    object OpenDialPad : PcpCallViewActions()
    data class SendDtmfDigit(val digit: String) : PcpCallViewActions()
    data class SwitchCall(val callArgs: CallArgs) : PcpCallViewActions()

    object SwitchSoundDevice : PcpCallViewActions()
    object HeadSetButtonPressed : PcpCallViewActions()
    object ToggleCamera : PcpCallViewActions()
    object ToggleHDSD : PcpCallViewActions()
    object InitiateCallTransfer : PcpCallViewActions()
    object CallTransferSelectionCancelled : PcpCallViewActions()
    data class CallTransferSelectionResult(val callTransferResult: CallTransferResult) : PcpCallViewActions()
    object TransferCall : PcpCallViewActions()
    object ToggleScreenSharing : PcpCallViewActions()
    data class StartScreenSharing(val videoCapturer: VideoCapturer) : PcpCallViewActions()
}
