/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.createdirect

import com.airbnb.mvrx.Loading
import com.airbnb.mvrx.MavericksViewModelFactory
import com.airbnb.mvrx.Success
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.mvrx.runCatchingToAsync
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.features.analytics.AnalyticsTracker
import im.vector.app.features.analytics.plan.CreatedRoom
import chat.pcp.app.features.raw.wellknown.getElementWellknown
import chat.pcp.app.features.raw.wellknown.isE2EByDefault
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.userdirectory.PendingSelection
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.extensions.orFalse
import org.matrix.android.sdk.api.raw.RawService
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.getUserOrDefault
import org.matrix.android.sdk.api.session.permalinks.PermalinkData
import org.matrix.android.sdk.api.session.permalinks.PermalinkParser
import org.matrix.android.sdk.api.session.room.model.create.CreateRoomParams

class CreateDirectRoomViewModel @AssistedInject constructor(
        @Assisted initialState: CreateDirectRoomViewState,
        private val rawService: RawService,
        private val pcpPreferences: PcpBasePreferences,
        val session: Session,
        val analyticsTracker: AnalyticsTracker,
) :
        PcpViewModel<CreateDirectRoomViewState, CreateDirectRoomAction, CreateDirectRoomViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<CreateDirectRoomViewModel, CreateDirectRoomViewState> {
        override fun create(initialState: CreateDirectRoomViewState): CreateDirectRoomViewModel
    }

    companion object : MavericksViewModelFactory<CreateDirectRoomViewModel, CreateDirectRoomViewState> by hiltMavericksViewModelFactory()

    override fun handle(action: CreateDirectRoomAction) {
        when (action) {
            is CreateDirectRoomAction.PrepareRoomWithSelectedUsers -> onSubmitInvitees(action.selections)
            is CreateDirectRoomAction.CreateRoomAndInviteSelectedUsers -> onCreateRoomWithInvitees()
            is CreateDirectRoomAction.QrScannedAction -> onCodeParsed(action)
        }
    }

    private fun onCodeParsed(action: CreateDirectRoomAction.QrScannedAction) {
        val mxid = (PermalinkParser.parse(action.result) as? PermalinkData.UserLink)?.userId

        if (mxid === null) {
            _viewEvents.post(CreateDirectRoomViewEvents.InvalidCode)
        } else {
            // The following assumes MXIDs are case insensitive
            if (mxid.equals(other = session.myUserId, ignoreCase = true)) {
                _viewEvents.post(CreateDirectRoomViewEvents.DmSelf)
            } else {
                // Try to get user from known users and fall back to creating a User object from MXID
                val qrInvitee = session.getUserOrDefault(mxid)
                onSubmitInvitees(setOf(PendingSelection.UserPendingSelection(qrInvitee)))
            }
        }
    }

    /**
     * If users already have a DM room then navigate to it instead of creating a new room.
     */
    private fun onSubmitInvitees(selections: Set<PendingSelection>) {
        val existingRoomId = selections.singleOrNull()?.getMxId()?.let { userId ->
            session.roomService().getExistingDirectRoomWithUser(userId)
        }
        if (existingRoomId != null) {
            // Do not create a new DM, just tell that the creation is successful by passing the existing roomId
            setState { copy(createAndInviteState = Success(existingRoomId)) }
        } else {
            createLocalRoomWithSelectedUsers(selections)
        }
    }

    private fun onCreateRoomWithInvitees() {
        // Create the DM
        withState { createLocalRoomWithSelectedUsers(it.pendingSelections) }
    }

    private fun createLocalRoomWithSelectedUsers(selections: Set<PendingSelection>) {
        setState { copy(createAndInviteState = Loading()) }

        viewModelScope.launch(Dispatchers.IO) {
            val adminE2EByDefault = rawService.getElementWellknown(session.sessionParams)
                    ?.isE2EByDefault()
                    ?: true

            val roomParams = CreateRoomParams()
                    .apply {
                        selections.forEach {
                            when (it) {
                                is PendingSelection.UserPendingSelection -> invitedUserIds.add(it.user.userId)
                                is PendingSelection.ThreePidPendingSelection -> invite3pids.add(it.threePid)
                            }
                        }
                        setDirectMessage()
                        enableEncryptionIfInvitedUsersSupportIt = adminE2EByDefault
                    }

            val result = runCatchingToAsync {
                if (pcpPreferences.isDeferredDmEnabled() && roomParams.invite3pids.isEmpty()) {
                    session.roomService().createLocalRoom(roomParams)
                } else {
                    analyticsTracker.capture(CreatedRoom(isDM = roomParams.isDirect.orFalse()))
                    session.roomService().createRoom(roomParams)
                }
            }

            setState {
                copy(
                        createAndInviteState = result
                )
            }
        }
    }
}
