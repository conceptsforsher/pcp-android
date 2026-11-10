/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.invite

import com.airbnb.mvrx.Async
import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.Uninitialized

data class InviteUsersToRoomViewState(
        val roomId: String,
        val inviteState: Async<Unit> = Uninitialized
) : MavericksState {

    constructor(args: InviteUsersToRoomArgs) : this(roomId = args.roomId)
}
