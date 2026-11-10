/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.composer.voice

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import com.airbnb.mvrx.parentFragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.hardware.vibrate
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.utils.PERMISSIONS_FOR_VOICE_MESSAGE
import chat.pcp.app.core.utils.checkPermissions
import chat.pcp.app.core.utils.onPermissionDeniedSnackbar
import chat.pcp.app.core.utils.registerForPermissionsResult
import chat.pcp.app.databinding.FragmentVoiceRecorderBinding
import chat.pcp.app.features.home.room.detail.TimelineViewModel
import chat.pcp.app.features.home.room.detail.composer.MessageComposerAction
import chat.pcp.app.features.home.room.detail.composer.MessageComposerViewEvents
import chat.pcp.app.features.home.room.detail.composer.MessageComposerViewModel
import chat.pcp.app.features.home.room.detail.composer.MessageComposerViewState
import chat.pcp.app.features.home.room.detail.composer.SendMode
import chat.pcp.app.features.home.room.detail.composer.boolean
import chat.pcp.app.features.home.room.detail.timeline.helper.AudioMessagePlaybackTracker
import chat.pcp.lib.core.utils.timer.Clock
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

@AndroidEntryPoint
class VoiceRecorderFragment : PcpFragment<FragmentVoiceRecorderBinding>() {

    @Inject lateinit var audioMessagePlaybackTracker: AudioMessagePlaybackTracker
    @Inject lateinit var clock: Clock

    private val timelineViewModel: TimelineViewModel by parentFragmentViewModel()
    private val messageComposerViewModel: MessageComposerViewModel by parentFragmentViewModel()

    private val permissionVoiceMessageLauncher = registerForPermissionsResult { allGranted, deniedPermanently ->
        if (allGranted) {
            // In this case, let the user start again the gesture
        } else if (deniedPermanently) {
            vectorBaseActivity.onPermissionDeniedSnackbar(CommonStrings.denied_permission_voice_message)
        }
    }

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentVoiceRecorderBinding {
        return FragmentVoiceRecorderBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        messageComposerViewModel.observeViewEvents {
            when (it) {
                is MessageComposerViewEvents.AnimateSendButtonVisibility -> handleSendButtonVisibilityChanged(it.isVisible)
                else -> Unit
            }
        }

        messageComposerViewModel.onEach(MessageComposerViewState::sendMode, MessageComposerViewState::canSendMessage) { mode, canSend ->
            if (!canSend.boolean()) {
                return@onEach
            }
            if (mode is SendMode.Voice) {
                views.voiceMessageRecorderView.isVisible = true
            }
        }
    }

    override fun onResume() {
        super.onResume()

        // Removed listeners should be set again
        setupVoiceMessageView()
    }

    override fun onPause() {
        super.onPause()

        audioMessagePlaybackTracker.pauseAllPlaybacks()
    }

    override fun invalidate() = withState(timelineViewModel, messageComposerViewModel) { mainState, messageComposerState ->
        if (mainState.tombstoneEvent != null) return@withState

        val hasVoiceDraft = messageComposerState.voiceRecordingUiState is VoiceMessageRecorderView.RecordingUiState.Draft
        with(views.root) {
            isVisible = messageComposerState.isVoiceMessageRecorderVisible || hasVoiceDraft
            render(messageComposerState.voiceRecordingUiState)
        }
    }

    private fun handleSendButtonVisibilityChanged(isSendButtonVisible: Boolean) {
        if (isSendButtonVisible) {
            views.root.isVisible = false
        } else {
            views.root.alpha = 0f
            views.root.isVisible = true
            views.root.animate().alpha(1f).setDuration(150).start()
        }
    }

    private fun setupVoiceMessageView() {
        audioMessagePlaybackTracker.track(AudioMessagePlaybackTracker.RECORDING_ID, views.voiceMessageRecorderView)
        views.voiceMessageRecorderView.callback = object : VoiceMessageRecorderView.Callback {

            override fun onVoiceRecordingStarted() {
                if (checkPermissions(PERMISSIONS_FOR_VOICE_MESSAGE, requireActivity(), permissionVoiceMessageLauncher)) {
                    messageComposerViewModel.handle(MessageComposerAction.StartRecordingVoiceMessage)
                    vibrate(requireContext())
                }
            }

            override fun onVoicePlaybackButtonClicked() {
                messageComposerViewModel.handle(MessageComposerAction.PlayOrPauseRecordingPlayback)
            }

            override fun onVoiceRecordingCancelled() {
                messageComposerViewModel.handle(MessageComposerAction.EndRecordingVoiceMessage(isCancelled = true, rootThreadEventId = getRootThreadEventId()))
                vibrate(requireContext())
                updateRecordingUiState(VoiceMessageRecorderView.RecordingUiState.Idle)
            }

            override fun onVoiceRecordingLocked() {
                val startedState = withState(messageComposerViewModel) { it.voiceRecordingUiState as? VoiceMessageRecorderView.RecordingUiState.Recording }
                val startTime = startedState?.recordingStartTimestamp ?: clock.epochMillis()
                updateRecordingUiState(VoiceMessageRecorderView.RecordingUiState.Locked(startTime))
            }

            override fun onVoiceRecordingEnded() {
                onSendVoiceMessage()
            }

            override fun onSendVoiceMessage() {
                messageComposerViewModel.handle(
                        MessageComposerAction.EndRecordingVoiceMessage(isCancelled = false, rootThreadEventId = getRootThreadEventId())
                )
                updateRecordingUiState(VoiceMessageRecorderView.RecordingUiState.Idle)
            }

            override fun onDeleteVoiceMessage() {
                messageComposerViewModel.handle(
                        MessageComposerAction.EndRecordingVoiceMessage(isCancelled = true, rootThreadEventId = getRootThreadEventId())
                )
                updateRecordingUiState(VoiceMessageRecorderView.RecordingUiState.Idle)
            }

            override fun onRecordingLimitReached() = pauseRecording()

            override fun onRecordingWaveformClicked() = pauseRecording()

            override fun onVoiceWaveformTouchedUp(percentage: Float, duration: Int) {
                messageComposerViewModel.handle(
                        MessageComposerAction.VoiceWaveformTouchedUp(AudioMessagePlaybackTracker.RECORDING_ID, duration, percentage)
                )
            }

            override fun onVoiceWaveformMoved(percentage: Float, duration: Int) {
                messageComposerViewModel.handle(
                        MessageComposerAction.VoiceWaveformTouchedUp(AudioMessagePlaybackTracker.RECORDING_ID, duration, percentage)
                )
            }

            private fun updateRecordingUiState(state: VoiceMessageRecorderView.RecordingUiState) {
                messageComposerViewModel.handle(
                        MessageComposerAction.OnVoiceRecordingUiStateChanged(state)
                )
            }

            private fun pauseRecording() {
                messageComposerViewModel.handle(
                        MessageComposerAction.PauseRecordingVoiceMessage
                )
                updateRecordingUiState(VoiceMessageRecorderView.RecordingUiState.Draft)
            }
        }
    }

    /**
     * Returns the root thread event if we are in a thread room, otherwise returns null.
     */
    fun getRootThreadEventId(): String? = withState(timelineViewModel) { it.rootThreadEventId }
}
