/*
 * Copyright 2023, 2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls.detail.domain

import chat.pcp.app.core.di.ActiveSessionHolder
import org.matrix.android.sdk.api.session.events.model.RelationType
import org.matrix.android.sdk.api.session.events.model.isPollEnd
import timber.log.Timber
import javax.inject.Inject

class GetEndedPollEventIdUseCase @Inject constructor(
        private val activeSessionHolder: ActiveSessionHolder,
) {

    fun execute(roomId: String, startPollEventId: String): String? {
        val result = runCatching {
            activeSessionHolder.getActiveSession().roomService().getRoom(roomId)
                    ?.timelineService()
                    ?.getTimelineEventsRelatedTo(RelationType.REFERENCE, startPollEventId)
                    ?.find { it.root.isPollEnd() }
                    ?.eventId
        }.onFailure { Timber.w("failed to retrieve the ended poll event id for eventId:$startPollEventId") }
        return result.getOrNull()
    }
}
