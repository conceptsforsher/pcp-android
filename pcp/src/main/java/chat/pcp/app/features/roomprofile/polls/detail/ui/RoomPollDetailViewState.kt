/*
 * Copyright 2026, 2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls.detail.ui

import com.airbnb.mvrx.MavericksState

data class RoomPollDetailViewState(
        val pollId: String,
        val roomId: String,
        val pollDetail: RoomPollDetail? = null,
) : MavericksState {

    constructor(roomPollDetailArgs: RoomPollDetailArgs) : this(
            pollId = roomPollDetailArgs.pollId,
            roomId = roomPollDetailArgs.roomId,
    )
}
