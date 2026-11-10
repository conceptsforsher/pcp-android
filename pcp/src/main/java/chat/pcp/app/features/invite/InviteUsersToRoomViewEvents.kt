/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.invite

import chat.pcp.app.core.platform.PcpViewEvents

sealed class InviteUsersToRoomViewEvents : PcpViewEvents {
    object Loading : InviteUsersToRoomViewEvents()
    data class Failure(val throwable: Throwable) : InviteUsersToRoomViewEvents()
    data class Success(val successMessage: String) : InviteUsersToRoomViewEvents()
}
