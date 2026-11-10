/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.usercode

import chat.pcp.app.core.platform.PcpViewEvents

sealed class UserCodeShareViewEvents : PcpViewEvents {
    object Dismiss : UserCodeShareViewEvents()
    object ShowWaitingScreen : UserCodeShareViewEvents()
    object HideWaitingScreen : UserCodeShareViewEvents()
    data class ToastMessage(val message: String) : UserCodeShareViewEvents()
    data class NavigateToRoom(val roomId: String) : UserCodeShareViewEvents()
    data class CameraPermissionNotGranted(val deniedPermanently: Boolean) : UserCodeShareViewEvents()
    data class SharePlainText(val text: String, val title: String, val richPlainText: String) : UserCodeShareViewEvents()
}
