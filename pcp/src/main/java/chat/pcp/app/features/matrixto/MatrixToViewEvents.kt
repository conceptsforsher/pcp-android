/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.matrixto

import chat.pcp.app.core.platform.PcpViewEvents

sealed class MatrixToViewEvents : PcpViewEvents {
    data class NavigateToRoom(val roomId: String) : MatrixToViewEvents()
    data class NavigateToSpace(val spaceId: String) : MatrixToViewEvents()
    data class ShowModalError(val error: String) : MatrixToViewEvents()
    object Dismiss : MatrixToViewEvents()
}
