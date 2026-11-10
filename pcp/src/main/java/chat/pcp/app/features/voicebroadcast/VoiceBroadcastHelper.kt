/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.voicebroadcast

import chat.pcp.app.features.voicebroadcast.listening.VoiceBroadcastPlayer
import chat.pcp.app.features.voicebroadcast.model.VoiceBroadcast
import chat.pcp.app.features.voicebroadcast.recording.usecase.PauseVoiceBroadcastUseCase
import chat.pcp.app.features.voicebroadcast.recording.usecase.ResumeVoiceBroadcastUseCase
import chat.pcp.app.features.voicebroadcast.recording.usecase.StartVoiceBroadcastUseCase
import chat.pcp.app.features.voicebroadcast.recording.usecase.StopVoiceBroadcastUseCase
import javax.inject.Inject

/**
 * Helper class to record voice broadcast.
 */
class VoiceBroadcastHelper @Inject constructor(
        private val startVoiceBroadcastUseCase: StartVoiceBroadcastUseCase,
        private val pauseVoiceBroadcastUseCase: PauseVoiceBroadcastUseCase,
        private val resumeVoiceBroadcastUseCase: ResumeVoiceBroadcastUseCase,
        private val stopVoiceBroadcastUseCase: StopVoiceBroadcastUseCase,
        private val voiceBroadcastPlayer: VoiceBroadcastPlayer,
) {
    suspend fun startVoiceBroadcast(roomId: String) = startVoiceBroadcastUseCase.execute(roomId)

    suspend fun pauseVoiceBroadcast(roomId: String) = pauseVoiceBroadcastUseCase.execute(roomId)

    suspend fun resumeVoiceBroadcast(roomId: String) = resumeVoiceBroadcastUseCase.execute(roomId)

    suspend fun stopVoiceBroadcast(roomId: String) = stopVoiceBroadcastUseCase.execute(roomId)

    fun playOrResumePlayback(voiceBroadcast: VoiceBroadcast) = voiceBroadcastPlayer.playOrResume(voiceBroadcast)

    fun pausePlayback() = voiceBroadcastPlayer.pause()

    fun stopPlayback() = voiceBroadcastPlayer.stop()

    fun seekTo(voiceBroadcast: VoiceBroadcast, positionMillis: Int, duration: Int) {
        voiceBroadcastPlayer.seekTo(voiceBroadcast, positionMillis, duration)
    }
}
