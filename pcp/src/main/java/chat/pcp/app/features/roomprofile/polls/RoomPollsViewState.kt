/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls

import com.airbnb.mvrx.MavericksState
import chat.pcp.app.features.roomprofile.RoomProfileArgs
import chat.pcp.app.features.roomprofile.polls.list.ui.PollSummary

data class RoomPollsViewState(
        val roomId: String,
        val polls: List<PollSummary> = emptyList(),
        val isLoadingMore: Boolean = false,
        val canLoadMore: Boolean = true,
        val nbSyncedDays: Int = 0,
        val isSyncing: Boolean = false,
) : MavericksState {

    constructor(roomProfileArgs: RoomProfileArgs) : this(roomId = roomProfileArgs.roomId)

    fun hasNoPolls() = polls.isEmpty()
    fun hasNoPollsAndCanLoadMore() = !isSyncing && hasNoPolls() && canLoadMore
}
