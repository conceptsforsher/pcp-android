/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

import org.matrix.android.sdk.api.session.room.model.RoomSummary
import org.matrix.android.sdk.api.session.room.model.SpaceChildInfo

interface RoomListListener {
    fun onRoomClicked(room: RoomSummary)
    fun onRoomLongClicked(room: RoomSummary): Boolean
    fun onRejectRoomInvitation(room: RoomSummary)
    fun onAcceptRoomInvitation(room: RoomSummary)
    fun onJoinSuggestedRoom(room: SpaceChildInfo)
    fun onSuggestedRoomClicked(room: SpaceChildInfo)
}
