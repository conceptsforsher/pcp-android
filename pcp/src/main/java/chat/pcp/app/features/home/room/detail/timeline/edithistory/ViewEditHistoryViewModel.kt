/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.home.room.detail.timeline.edithistory

import com.airbnb.mvrx.Fail
import com.airbnb.mvrx.Loading
import com.airbnb.mvrx.MavericksViewModelFactory
import com.airbnb.mvrx.Success
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.EmptyAction
import chat.pcp.app.core.platform.EmptyViewEvents
import chat.pcp.app.core.platform.PcpViewModel
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.crypto.MXCryptoError
import org.matrix.android.sdk.api.session.crypto.model.OlmDecryptionResult
import org.matrix.android.sdk.api.session.events.model.isReply
import org.matrix.android.sdk.api.session.getRoom
import timber.log.Timber
import java.util.UUID

class ViewEditHistoryViewModel @AssistedInject constructor(
        @Assisted initialState: ViewEditHistoryViewState,
        private val session: Session
) : PcpViewModel<ViewEditHistoryViewState, EmptyAction, EmptyViewEvents>(initialState) {

    private val roomId = initialState.roomId
    private val eventId = initialState.eventId
    private val room = session.getRoom(roomId)
            ?: throw IllegalStateException("Shouldn't use this ViewModel without a room")

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<ViewEditHistoryViewModel, ViewEditHistoryViewState> {
        override fun create(initialState: ViewEditHistoryViewState): ViewEditHistoryViewModel
    }

    companion object : MavericksViewModelFactory<ViewEditHistoryViewModel, ViewEditHistoryViewState> by hiltMavericksViewModelFactory()

    init {
        loadHistory()
    }

    private fun loadHistory() {
        setState { copy(editList = Loading()) }

        viewModelScope.launch {
            val data = try {
                room.relationService().fetchEditHistory(eventId)
            } catch (failure: Throwable) {
                setState {
                    copy(editList = Fail(failure))
                }
                return@launch
            }

            var originalIsReply = false

            data.forEach { event ->
                val timelineID = event.roomId + UUID.randomUUID().toString()
                // We need to check encryption
                if (event.isEncrypted() && event.mxDecryptionResult == null) {
                    // for now decrypt sync
                    try {
                        val result = session.cryptoService().decryptEvent(event, timelineID)
                        event.mxDecryptionResult = OlmDecryptionResult(
                                payload = result.clearEvent,
                                senderKey = result.senderCurve25519Key,
                                keysClaimed = result.claimedEd25519Key?.let { k -> mapOf("ed25519" to k) },
                                forwardingCurve25519KeyChain = result.forwardingCurve25519KeyChain,
                                verificationState = result.messageVerificationState
                        )
                    } catch (e: MXCryptoError) {
                        Timber.w("Failed to decrypt event in history")
                    }
                }

                if (event.eventId == eventId) {
                    originalIsReply = event.isReply()
                }
            }
            setState {
                copy(
                        editList = Success(data),
                        isOriginalAReply = originalIsReply
                )
            }
        }
    }

    override fun handle(action: EmptyAction) {
        // No op
    }
}
