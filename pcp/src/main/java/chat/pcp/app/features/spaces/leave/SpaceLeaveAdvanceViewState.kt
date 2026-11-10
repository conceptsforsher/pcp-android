/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.leave

import com.airbnb.mvrx.Async
import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.Uninitialized
import chat.pcp.app.features.spaces.SpaceBottomSheetSettingsArgs
import org.matrix.android.sdk.api.session.room.model.RoomSummary

data class SpaceLeaveAdvanceViewState(
        val spaceId: String,
        val spaceSummary: RoomSummary? = null,
        val allChildren: Async<List<RoomSummary>> = Uninitialized,
        val selectedRooms: List<String> = emptyList(),
        val currentFilter: String = "",
        val leaveState: Async<Unit> = Uninitialized,
        val isFilteringEnabled: Boolean = false,
        val isLastAdmin: Boolean = false
) : MavericksState {

    constructor(args: SpaceBottomSheetSettingsArgs) : this(
            spaceId = args.spaceId
    )
}
