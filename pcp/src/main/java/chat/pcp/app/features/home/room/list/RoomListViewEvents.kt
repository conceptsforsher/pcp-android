/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.session.room.model.RoomSummary

/**
 * Transient events for RoomList.
 */
sealed class RoomListViewEvents : PcpViewEvents {
    data class Loading(val message: CharSequence? = null) : RoomListViewEvents()
    data class Failure(val throwable: Throwable) : RoomListViewEvents()

    data class SelectRoom(val roomSummary: RoomSummary, val isInviteAlreadyAccepted: Boolean = false) : RoomListViewEvents()
    object Done : RoomListViewEvents()
    data class NavigateToMxToBottomSheet(val link: String) : RoomListViewEvents()
}
