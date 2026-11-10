/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home.invites

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.session.room.model.RoomSummary

sealed class InvitesViewEvents : PcpViewEvents {
    data class Failure(val throwable: Throwable) : InvitesViewEvents()
    data class OpenRoom(
            val roomSummary: RoomSummary,
            val shouldCloseInviteView: Boolean,
            val isInviteAlreadySelected: Boolean,
    ) : InvitesViewEvents()
}
