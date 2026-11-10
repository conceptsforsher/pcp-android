/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.home.room.detail.timeline.factory

import chat.pcp.app.core.error.ErrorFormatter
import chat.pcp.app.core.resources.ColorProvider
import chat.pcp.app.core.resources.DrawableProvider
import chat.pcp.app.features.home.room.detail.timeline.helper.AudioMessagePlaybackTracker
import chat.pcp.app.features.home.room.detail.timeline.helper.AvatarSizeProvider
import chat.pcp.app.features.home.room.detail.timeline.helper.VoiceBroadcastEventsGroup
import chat.pcp.app.features.home.room.detail.timeline.item.AbsMessageItem
import chat.pcp.app.features.home.room.detail.timeline.item.AbsMessageVoiceBroadcastItem
import chat.pcp.app.features.home.room.detail.timeline.item.BaseEventItem
import chat.pcp.app.features.home.room.detail.timeline.item.MessageVoiceBroadcastListeningItem
import chat.pcp.app.features.home.room.detail.timeline.item.MessageVoiceBroadcastListeningItem_
import chat.pcp.app.features.home.room.detail.timeline.item.MessageVoiceBroadcastRecordingItem
import chat.pcp.app.features.home.room.detail.timeline.item.MessageVoiceBroadcastRecordingItem_
import chat.pcp.app.features.voicebroadcast.listening.VoiceBroadcastPlayer
import chat.pcp.app.features.voicebroadcast.model.MessageVoiceBroadcastInfoContent
import chat.pcp.app.features.voicebroadcast.model.VoiceBroadcast
import chat.pcp.app.features.voicebroadcast.model.VoiceBroadcastState
import chat.pcp.app.features.voicebroadcast.model.asVoiceBroadcastEvent
import chat.pcp.app.features.voicebroadcast.recording.VoiceBroadcastRecorder
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.getRoom
import org.matrix.android.sdk.api.util.toMatrixItem
import javax.inject.Inject

class VoiceBroadcastItemFactory @Inject constructor(
        private val session: Session,
        private val avatarSizeProvider: AvatarSizeProvider,
        private val colorProvider: ColorProvider,
        private val drawableProvider: DrawableProvider,
        private val errorFormatter: ErrorFormatter,
        private val voiceBroadcastRecorder: VoiceBroadcastRecorder?,
        private val voiceBroadcastPlayer: VoiceBroadcastPlayer,
        private val playbackTracker: AudioMessagePlaybackTracker,
        private val noticeItemFactory: NoticeItemFactory,
) {

    fun create(
            params: TimelineItemFactoryParams,
            messageContent: MessageVoiceBroadcastInfoContent,
            highlight: Boolean,
            attributes: AbsMessageItem.Attributes,
    ): BaseEventItem<*>? {
        // Only display item of the initial event with updated data
        if (messageContent.voiceBroadcastState != VoiceBroadcastState.STARTED) {
            return noticeItemFactory.create(params)
        }

        val voiceBroadcastEventsGroup = params.eventsGroup?.let { VoiceBroadcastEventsGroup(it) } ?: return null
        val voiceBroadcastEvent = voiceBroadcastEventsGroup.getLastDisplayableEvent().root.asVoiceBroadcastEvent() ?: return null
        val voiceBroadcastContent = voiceBroadcastEvent.content ?: return null
        val voiceBroadcast = VoiceBroadcast(voiceBroadcastId = voiceBroadcastEventsGroup.voiceBroadcastId, roomId = params.event.roomId)

        val isRecording = voiceBroadcastContent.voiceBroadcastState != VoiceBroadcastState.STOPPED &&
                voiceBroadcastEvent.root.stateKey == session.myUserId &&
                messageContent.deviceId == session.sessionParams.deviceId

        val voiceBroadcastAttributes = AbsMessageVoiceBroadcastItem.Attributes(
                voiceBroadcast = voiceBroadcast,
                voiceBroadcastState = voiceBroadcastContent.voiceBroadcastState,
                duration = voiceBroadcastEventsGroup.getDuration(),
                hasUnableToDecryptEvent = voiceBroadcastEventsGroup.hasUnableToDecryptEvent(),
                recorderName = params.event.senderInfo.disambiguatedDisplayName,
                recorder = voiceBroadcastRecorder,
                player = voiceBroadcastPlayer,
                playbackTracker = playbackTracker,
                roomItem = session.getRoom(params.event.roomId)?.roomSummary()?.toMatrixItem(),
                colorProvider = colorProvider,
                drawableProvider = drawableProvider,
                errorFormatter = errorFormatter,
        )

        return if (isRecording) {
            createRecordingItem(highlight, attributes, voiceBroadcastAttributes)
        } else {
            createListeningItem(highlight, attributes, voiceBroadcastAttributes)
        }
    }

    private fun createRecordingItem(
            highlight: Boolean,
            attributes: AbsMessageItem.Attributes,
            voiceBroadcastAttributes: AbsMessageVoiceBroadcastItem.Attributes,
    ): MessageVoiceBroadcastRecordingItem {
        return MessageVoiceBroadcastRecordingItem_()
                .id("voice_broadcast_${voiceBroadcastAttributes.voiceBroadcast.voiceBroadcastId}")
                .attributes(attributes)
                .voiceBroadcastAttributes(voiceBroadcastAttributes)
                .highlighted(highlight)
                .leftGuideline(avatarSizeProvider.leftGuideline)
    }

    private fun createListeningItem(
            highlight: Boolean,
            attributes: AbsMessageItem.Attributes,
            voiceBroadcastAttributes: AbsMessageVoiceBroadcastItem.Attributes,
    ): MessageVoiceBroadcastListeningItem {
        return MessageVoiceBroadcastListeningItem_()
                .id("voice_broadcast_${voiceBroadcastAttributes.voiceBroadcast.voiceBroadcastId}")
                .attributes(attributes)
                .voiceBroadcastAttributes(voiceBroadcastAttributes)
                .highlighted(highlight)
                .leftGuideline(avatarSizeProvider.leftGuideline)
    }
}
