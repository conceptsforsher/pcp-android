/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.threads.list.viewmodel

import com.airbnb.mvrx.Async
import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.Uninitialized
import chat.pcp.app.features.home.room.threads.arguments.ThreadListArgs
import org.matrix.android.sdk.api.session.threads.ThreadTimelineEvent

data class ThreadListViewState(
        val rootThreadEventList: Async<List<ThreadTimelineEvent>> = Uninitialized,
        val shouldFilterThreads: Boolean = false,
        val isLoading: Boolean = false,
        val roomId: String
) : MavericksState {
    constructor(args: ThreadListArgs) : this(roomId = args.roomId)
}
