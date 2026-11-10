/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call.utils

import org.matrix.android.sdk.api.session.room.model.call.CallCandidate
import org.matrix.android.sdk.api.session.room.model.call.SdpType
import org.webrtc.IceCandidate
import org.webrtc.SessionDescription

fun List<IceCandidate>.mapToCallCandidate() = map {
    CallCandidate(
            sdpMid = it.sdpMid,
            sdpMLineIndex = it.sdpMLineIndex,
            candidate = it.sdp
    )
}

fun SdpType.asWebRTC(): SessionDescription.Type {
    return if (this == SdpType.OFFER) {
        SessionDescription.Type.OFFER
    } else {
        SessionDescription.Type.ANSWER
    }
}
