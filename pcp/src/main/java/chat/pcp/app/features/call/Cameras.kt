/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call

enum class CameraType {
    FRONT,
    BACK
}

data class CameraProxy(
        val name: String,
        val type: CameraType
)

sealed class CaptureFormat(val width: Int, val height: Int, val fps: Int) {
    object HD : CaptureFormat(1280, 720, 30)
    object SD : CaptureFormat(640, 480, 30)
}
