/*
 * Copyright 2026, 2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls.list.data

import androidx.lifecycle.asFlow
import chat.pcp.app.core.di.ActiveSessionHolder
import kotlinx.coroutines.flow.Flow
import org.matrix.android.sdk.api.session.getRoom
import org.matrix.android.sdk.api.session.room.poll.LoadedPollsStatus
import org.matrix.android.sdk.api.session.room.poll.PollHistoryService
import org.matrix.android.sdk.api.session.room.timeline.TimelineEvent
import javax.inject.Inject

class RoomPollDataSource @Inject constructor(
        private val activeSessionHolder: ActiveSessionHolder,
) {

    private fun getPollHistoryService(roomId: String): PollHistoryService {
        return activeSessionHolder
                .getSafeActiveSession()
                ?.getRoom(roomId)
                ?.pollHistoryService()
                ?: throw PollHistoryError.UnknownRoomError
    }

    fun dispose(roomId: String) {
        getPollHistoryService(roomId).dispose()
    }

    fun getPolls(roomId: String): Flow<List<TimelineEvent>> {
        return getPollHistoryService(roomId).getPollEvents().asFlow()
    }

    suspend fun getLoadedPollsStatus(roomId: String): LoadedPollsStatus {
        return getPollHistoryService(roomId).getLoadedPollsStatus()
    }

    suspend fun loadMorePolls(roomId: String): LoadedPollsStatus {
        return getPollHistoryService(roomId).loadMore()
    }

    suspend fun syncPolls(roomId: String) {
        getPollHistoryService(roomId).syncPolls()
    }
}
