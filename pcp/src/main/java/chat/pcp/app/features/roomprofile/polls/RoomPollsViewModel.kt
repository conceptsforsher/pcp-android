/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.features.roomprofile.polls.list.domain.DisposePollHistoryUseCase
import chat.pcp.app.features.roomprofile.polls.list.domain.GetPollsUseCase
import chat.pcp.app.features.roomprofile.polls.list.domain.LoadMorePollsUseCase
import chat.pcp.app.features.roomprofile.polls.list.domain.SyncPollsUseCase
import chat.pcp.app.features.roomprofile.polls.list.ui.PollSummaryMapper
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.flow.onEach
import kotlinx.coroutines.launch

class RoomPollsViewModel @AssistedInject constructor(
        @Assisted initialState: RoomPollsViewState,
        private val getPollsUseCase: GetPollsUseCase,
        private val loadMorePollsUseCase: LoadMorePollsUseCase,
        private val syncPollsUseCase: SyncPollsUseCase,
        private val disposePollHistoryUseCase: DisposePollHistoryUseCase,
        private val pollSummaryMapper: PollSummaryMapper,
) : PcpViewModel<RoomPollsViewState, RoomPollsAction, RoomPollsViewEvent>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<RoomPollsViewModel, RoomPollsViewState> {
        override fun create(initialState: RoomPollsViewState): RoomPollsViewModel
    }

    companion object : MavericksViewModelFactory<RoomPollsViewModel, RoomPollsViewState> by hiltMavericksViewModelFactory()

    init {
        val roomId = initialState.roomId
        syncPolls(roomId)
        observePolls(roomId)
    }

    override fun onCleared() {
        withState { disposePollHistoryUseCase.execute(it.roomId) }
        super.onCleared()
    }

    private fun syncPolls(roomId: String) {
        viewModelScope.launch {
            setState { copy(isSyncing = true) }
            val result = runCatching {
                val loadedPollsStatus = syncPollsUseCase.execute(roomId)
                setState {
                    copy(
                            canLoadMore = loadedPollsStatus.canLoadMore,
                            nbSyncedDays = loadedPollsStatus.daysSynced,
                    )
                }
            }
            if (result.isFailure) {
                _viewEvents.post(RoomPollsViewEvent.LoadingError)
            }
            setState { copy(isSyncing = false) }
        }
    }

    private fun observePolls(roomId: String) {
        getPollsUseCase.execute(roomId)
                .map { it.mapNotNull { event -> pollSummaryMapper.map(event) } }
                .onEach { setState { copy(polls = it) } }
                .launchIn(viewModelScope)
    }

    override fun handle(action: RoomPollsAction) {
        when (action) {
            RoomPollsAction.LoadMorePolls -> handleLoadMore()
        }
    }

    private fun handleLoadMore() = withState { viewState ->
        viewModelScope.launch {
            setState { copy(isLoadingMore = true) }
            val result = runCatching {
                val status = loadMorePollsUseCase.execute(viewState.roomId)
                setState {
                    copy(
                            canLoadMore = status.canLoadMore,
                            nbSyncedDays = status.daysSynced,
                    )
                }
            }
            if (result.isFailure) {
                _viewEvents.post(RoomPollsViewEvent.LoadingError)
            }
            setState { copy(isLoadingMore = false) }
        }
    }
}
