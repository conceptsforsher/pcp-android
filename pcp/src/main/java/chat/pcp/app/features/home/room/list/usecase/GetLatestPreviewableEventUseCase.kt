/*
 * Copyright 2023, 2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.usecase

import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.voicebroadcast.isLive
import chat.pcp.app.features.voicebroadcast.isVoiceBroadcast
import chat.pcp.app.features.voicebroadcast.model.asVoiceBroadcastEvent
import chat.pcp.app.features.voicebroadcast.usecase.GetRoomLiveVoiceBroadcastsUseCase
import chat.pcp.app.features.voicebroadcast.voiceBroadcastId
import org.matrix.android.sdk.api.extensions.orFalse
import org.matrix.android.sdk.api.session.events.model.EventType
import org.matrix.android.sdk.api.session.getRoom
import org.matrix.android.sdk.api.session.room.Room
import org.matrix.android.sdk.api.session.room.getTimelineEvent
import org.matrix.android.sdk.api.session.room.model.RoomSummary
import org.matrix.android.sdk.api.session.room.model.message.asMessageAudioEvent
import org.matrix.android.sdk.api.session.room.timeline.TimelineEvent
import javax.inject.Inject

class GetLatestPreviewableEventUseCase @Inject constructor(
        private val sessionHolder: ActiveSessionHolder,
        private val getRoomLiveVoiceBroadcastsUseCase: GetRoomLiveVoiceBroadcastsUseCase,
        private val pcpPreferences: PcpBasePreferences,
) {

    fun execute(roomId: String): TimelineEvent? {
        val room = sessionHolder.getSafeActiveSession()?.getRoom(roomId) ?: return null
        val roomSummary = room.roomSummary() ?: return null
        // FIXME Observing live broadcasts results in many db requests,
        //  to prevent performances issues, we only enable this mechanism if the voice broadcast flag is enabled
        return if (pcpPreferences.isVoiceBroadcastEnabled()) {
            getCallEvent(roomSummary)
                    ?: getLiveVoiceBroadcastEvent(room)
                    ?: getDefaultLatestEvent(room, roomSummary)
        } else {
            roomSummary.latestPreviewableEvent
        }
    }

    private fun getCallEvent(roomSummary: RoomSummary): TimelineEvent? {
        return roomSummary.latestPreviewableEvent
                ?.takeIf { it.root.getClearType() == EventType.CALL_INVITE }
    }

    private fun getLiveVoiceBroadcastEvent(room: Room): TimelineEvent? {
        return getRoomLiveVoiceBroadcastsUseCase.execute(room.roomId)
                .lastOrNull()
                ?.voiceBroadcastId
                ?.let { room.getTimelineEvent(it) }
    }

    private fun getDefaultLatestEvent(room: Room, roomSummary: RoomSummary): TimelineEvent? {
        val latestPreviewableEvent = roomSummary.latestPreviewableEvent

        // If the default latest event is a live voice broadcast (paused or resumed), rely to the started event
        val liveVoiceBroadcastEventId = latestPreviewableEvent?.root?.asVoiceBroadcastEvent()?.takeIf { it.isLive }?.voiceBroadcastId
        if (liveVoiceBroadcastEventId != null) {
            return room.getTimelineEvent(liveVoiceBroadcastEventId)
        }

        return latestPreviewableEvent
                ?.takeUnless { it.root.asMessageAudioEvent()?.isVoiceBroadcast().orFalse() } // Skip voice messages related to voice broadcast
    }
}
