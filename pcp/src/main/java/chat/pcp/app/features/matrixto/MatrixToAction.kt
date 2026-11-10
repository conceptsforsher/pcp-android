/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.matrixto

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.util.MatrixItem

sealed class MatrixToAction : PcpViewModelAction {
    data class StartChattingWithUser(val matrixItem: MatrixItem) : MatrixToAction()
    object FailedToResolveUser : MatrixToAction()
    object FailedToStartChatting : MatrixToAction()
    data class JoinSpace(val spaceID: String, val viaServers: List<String>?) : MatrixToAction()
    data class JoinRoom(val roomIdOrAlias: String, val viaServers: List<String>?) : MatrixToAction()
    data class OpenSpace(val spaceID: String) : MatrixToAction()
    data class OpenRoom(val roomId: String) : MatrixToAction()
}
