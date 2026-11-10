/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.usercode

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.util.MatrixItem

sealed class UserCodeActions : PcpViewModelAction {
    object DismissAction : UserCodeActions()
    data class SwitchMode(val mode: UserCodeState.Mode) : UserCodeActions()
    data class DecodedQRCode(val code: String) : UserCodeActions()
    data class StartChattingWithUser(val matrixItem: MatrixItem) : UserCodeActions()
    data class CameraPermissionNotGranted(val deniedPermanently: Boolean) : UserCodeActions()
    object ShareByText : UserCodeActions()
}
