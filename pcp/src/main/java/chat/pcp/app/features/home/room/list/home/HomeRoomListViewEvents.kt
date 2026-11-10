/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.session.room.model.RoomSummary

sealed class HomeRoomListViewEvents : PcpViewEvents {
    data class Loading(val message: CharSequence? = null) : HomeRoomListViewEvents()
    data class Failure(val throwable: Throwable) : HomeRoomListViewEvents()
    object Done : HomeRoomListViewEvents()
    data class SelectRoom(val roomSummary: RoomSummary, val isInviteAlreadyAccepted: Boolean = false) : HomeRoomListViewEvents()
}
