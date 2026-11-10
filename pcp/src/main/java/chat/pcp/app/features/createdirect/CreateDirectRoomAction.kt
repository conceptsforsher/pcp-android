/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.createdirect

import chat.pcp.app.core.platform.PcpViewModelAction
import chat.pcp.app.features.userdirectory.PendingSelection

sealed class CreateDirectRoomAction : PcpViewModelAction {
    data class PrepareRoomWithSelectedUsers(
            val selections: Set<PendingSelection>
    ) : CreateDirectRoomAction()

    object CreateRoomAndInviteSelectedUsers : CreateDirectRoomAction()

    data class QrScannedAction(
            val result: String
    ) : CreateDirectRoomAction()
}
