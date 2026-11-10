/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

import com.airbnb.mvrx.Async
import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.Uninitialized
import chat.pcp.app.features.home.RoomListDisplayMode
import org.matrix.android.sdk.api.session.room.members.ChangeMembershipState
import org.matrix.android.sdk.api.session.room.model.RoomSummary
import org.matrix.android.sdk.api.session.room.model.SpaceChildInfo

data class RoomListViewState(
        val displayMode: RoomListDisplayMode,
        val roomFilter: String = "",
        val roomMembershipChanges: Map<String, ChangeMembershipState> = emptyMap(),
        val asyncSuggestedRooms: Async<List<SpaceChildInfo>> = Uninitialized,
        val currentUserName: String? = null,
        val asyncSelectedSpace: Async<RoomSummary?> = Uninitialized,
) : MavericksState {

    constructor(args: RoomListParams) : this(displayMode = args.displayMode)
}
