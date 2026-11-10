/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.upgrade

import com.airbnb.mvrx.Loading
import com.airbnb.mvrx.MavericksViewModelFactory
import com.airbnb.mvrx.Success
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.EmptyViewEvents
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.features.session.coroutineScope
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.getRoom
import org.matrix.android.sdk.api.session.getRoomSummary

class MigrateRoomViewModel @AssistedInject constructor(
        @Assisted initialState: MigrateRoomViewState,
        private val session: Session,
        private val upgradeRoomViewModelTask: UpgradeRoomViewModelTask
) :
        PcpViewModel<MigrateRoomViewState, MigrateRoomAction, EmptyViewEvents>(initialState) {

    init {
        val room = session.getRoom(initialState.roomId)
        val summary = session.getRoomSummary(initialState.roomId)
        setState {
            copy(
                    currentVersion = room?.roomVersionService()?.getRoomVersion(),
                    isPublic = summary?.isPublic ?: false,
                    otherMemberCount = summary?.otherMemberIds?.count() ?: 0,
                    knownParents = summary?.flattenParentIds ?: emptyList()
            )
        }
    }

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<MigrateRoomViewModel, MigrateRoomViewState> {
        override fun create(initialState: MigrateRoomViewState): MigrateRoomViewModel
    }

    companion object : MavericksViewModelFactory<MigrateRoomViewModel, MigrateRoomViewState> by hiltMavericksViewModelFactory()

    override fun handle(action: MigrateRoomAction) {
        when (action) {
            is MigrateRoomAction.SetAutoInvite -> {
                setState {
                    copy(shouldIssueInvites = action.autoInvite)
                }
            }
            is MigrateRoomAction.SetUpdateKnownParentSpace -> {
                setState {
                    copy(shouldUpdateKnownParents = action.update)
                }
            }
            MigrateRoomAction.UpgradeRoom -> {
                handleUpgradeRoom()
            }
        }
    }

    private fun handleUpgradeRoom() = withState { state ->
        val summary = session.getRoomSummary(state.roomId)
        setState {
            copy(upgradingStatus = Loading())
        }
        session.coroutineScope.launch {
            val userToInvite = if (state.autoMigrateMembersAndParents) {
                summary?.otherMemberIds?.takeIf { !state.isPublic }
            } else {
                summary?.otherMemberIds?.takeIf { state.shouldIssueInvites }
            }.orEmpty()

            val parentSpaceToUpdate = if (state.autoMigrateMembersAndParents) {
                summary?.flattenParentIds
            } else {
                summary?.flattenParentIds?.takeIf { state.shouldUpdateKnownParents }
            }.orEmpty()

            val result = upgradeRoomViewModelTask.execute(UpgradeRoomViewModelTask.Params(
                    roomId = state.roomId,
                    newVersion = state.newVersion,
                    userIdsToAutoInvite = userToInvite,
                    parentSpaceToUpdate = parentSpaceToUpdate,
                    progressReporter = { indeterminate, progress, total ->
                        setState {
                            copy(
                                    upgradingProgress = progress,
                                    upgradingProgressTotal = total,
                                    upgradingProgressIndeterminate = indeterminate
                            )
                        }
                    }
            ))

            setState {
                copy(upgradingStatus = Success(result))
            }
        }
    }
}
