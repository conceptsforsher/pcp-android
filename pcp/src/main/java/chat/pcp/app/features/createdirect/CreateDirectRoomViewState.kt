/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.createdirect

import com.airbnb.mvrx.Async
import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.Uninitialized
import chat.pcp.app.features.userdirectory.PendingSelection

data class CreateDirectRoomViewState(
        val pendingSelections: Set<PendingSelection> = emptySet(),
        val createAndInviteState: Async<String> = Uninitialized
) : MavericksState
