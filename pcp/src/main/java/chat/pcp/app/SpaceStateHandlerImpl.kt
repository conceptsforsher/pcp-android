/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app

import androidx.lifecycle.LifecycleOwner
import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.core.utils.BehaviorDataSource
import chat.pcp.app.features.analytics.AnalyticsTracker
import im.vector.app.features.analytics.plan.UserProperties
import im.vector.app.features.analytics.plan.ViewRoom
import chat.pcp.app.features.session.coroutineScope
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.ui.UiStateRepository
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.cancelChildren
import kotlinx.coroutines.flow.distinctUntilChanged
import kotlinx.coroutines.flow.filterIsInstance
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.flow.onEach
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.extensions.tryOrNull
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.getRoom
import org.matrix.android.sdk.api.session.getRoomSummary
import org.matrix.android.sdk.api.session.room.model.RoomSummary
import org.matrix.android.sdk.api.session.sync.SyncRequestState
import org.matrix.android.sdk.api.util.Optional
import org.matrix.android.sdk.api.util.toOption
import javax.inject.Inject
import javax.inject.Singleton

/**
 * This class handles the global app state.
 * It is required that this class is added as an observer to ProcessLifecycleOwner.get().lifecycle in [PcpApplication]
 */
@Singleton
class SpaceStateHandlerImpl @Inject constructor(
        private val sessionDataSource: ActiveSessionDataSource,
        private val uiStateRepository: UiStateRepository,
        private val activeSessionHolder: ActiveSessionHolder,
        private val analyticsTracker: AnalyticsTracker,
        private val pcpPreferences: PcpBasePreferences,
) : SpaceStateHandler {

    private val coroutineScope = CoroutineScope(SupervisorJob() + Dispatchers.Main)
    private val selectedSpaceDataSource = BehaviorDataSource<Optional<RoomSummary>>(Optional.empty())
    private val selectedSpaceFlow = selectedSpaceDataSource.stream()

    override fun getCurrentSpace(): RoomSummary? {
        return selectedSpaceDataSource.currentValue?.orNull()?.let { spaceSummary ->
            activeSessionHolder.getSafeActiveSession()?.roomService()?.getRoomSummary(spaceSummary.roomId)
        }
    }

    override fun setCurrentSpace(
            spaceId: String?,
            session: Session?,
            persistNow: Boolean,
            isForwardNavigation: Boolean,
    ) {
        val activeSession = session ?: activeSessionHolder.getSafeActiveSession() ?: return
        val spaceToLeave = selectedSpaceDataSource.currentValue?.orNull()
        val spaceToSet = spaceId?.let { activeSession.getRoomSummary(spaceId) }
        val sameSpaceSelected = spaceId == spaceToLeave?.roomId

        if (sameSpaceSelected) {
            return
        }

        analyticsTracker.capture(
                ViewRoom(
                        isDM = false,
                        isSpace = true,
                )
        )

        if (isForwardNavigation) {
            addToBackstack(spaceToLeave, spaceToSet)
        }

        if (persistNow) {
            uiStateRepository.storeSelectedSpace(spaceToSet?.roomId, activeSession.sessionId)
        }

        selectedSpaceDataSource.post(spaceToSet.toOption())

        if (spaceId != null) {
            activeSession.coroutineScope.launch(Dispatchers.IO) {
                tryOrNull {
                    activeSession.getRoom(spaceId)?.membershipService()?.loadRoomMembersIfNeeded()
                }
            }
        }
    }

    private fun addToBackstack(spaceToLeave: RoomSummary?, spaceToSet: RoomSummary?) {
        // Only add to the backstack if the space to set is not All Chats, else clear the backstack
        if (spaceToSet != null) {
            val currentPersistedBackstack = pcpPreferences.getSpaceBackstack().toMutableList()
            currentPersistedBackstack.add(spaceToLeave?.roomId)
            pcpPreferences.setSpaceBackstack(currentPersistedBackstack)
        } else {
            pcpPreferences.setSpaceBackstack(emptyList())
        }
    }

    private fun observeActiveSession() {
        sessionDataSource.stream()
                .distinctUntilChanged()
                .onEach {
                    // sessionDataSource could already return a session while activeSession holder still returns null
                    it.orNull()?.let { session ->
                        setCurrentSpace(uiStateRepository.getSelectedSpace(session.sessionId), session)
                        observeSyncStatus(session)
                    }
                }
                .launchIn(coroutineScope)
    }

    private fun observeSyncStatus(session: Session) {
        session.syncService().getSyncRequestStateFlow()
                .filterIsInstance<SyncRequestState.IncrementalSyncDone>()
                .map { session.spaceService().getRootSpaceSummaries().size }
                .distinctUntilChanged()
                .onEach { spacesNumber ->
                    analyticsTracker.updateUserProperties(UserProperties(numSpaces = spacesNumber))
                }.launchIn(session.coroutineScope)
    }

    override fun popSpaceBackstack(): String? {
        pcpPreferences.getSpaceBackstack().toMutableList().apply {
            val poppedSpaceId = removeAt(lastIndex)
            pcpPreferences.setSpaceBackstack(this)
            return poppedSpaceId
        }
    }

    override fun getSpaceBackstack() = pcpPreferences.getSpaceBackstack()

    override fun getSelectedSpaceFlow() = selectedSpaceFlow

    override fun getSafeActiveSpaceId(): String? {
        return selectedSpaceDataSource.currentValue?.orNull()?.roomId
    }

    override fun onResume(owner: LifecycleOwner) {
        observeActiveSession()
    }

    override fun onPause(owner: LifecycleOwner) {
        coroutineScope.coroutineContext.cancelChildren()
        val session = activeSessionHolder.getSafeActiveSession() ?: return
        uiStateRepository.storeSelectedSpace(selectedSpaceDataSource.currentValue?.orNull()?.roomId, session.sessionId)
    }
}
