/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home

import chat.pcp.app.core.platform.PcpViewModelAction
import chat.pcp.app.features.home.room.list.home.header.HomeRoomFilter
import org.matrix.android.sdk.api.session.room.model.RoomSummary
import org.matrix.android.sdk.api.session.room.notification.RoomNotificationState

sealed class HomeRoomListAction : PcpViewModelAction {
    data class SelectRoom(val roomSummary: RoomSummary) : HomeRoomListAction()
    data class ChangeRoomNotificationState(val roomId: String, val notificationState: RoomNotificationState) : HomeRoomListAction()
    data class ToggleTag(val roomId: String, val tag: String) : HomeRoomListAction()
    data class LeaveRoom(val roomId: String) : HomeRoomListAction()
    data class ChangeRoomFilter(val filter: HomeRoomFilter) : HomeRoomListAction()
    object DeleteAllLocalRoom : HomeRoomListAction()
}
