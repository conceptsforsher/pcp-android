/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home.release

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.PcpDummyViewState
import chat.pcp.app.core.platform.PcpViewModel

class ReleaseNotesViewModel @AssistedInject constructor(
        @Assisted initialState: PcpDummyViewState,
) : PcpViewModel<PcpDummyViewState, ReleaseNotesAction, ReleaseNotesViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<ReleaseNotesViewModel, PcpDummyViewState> {
        override fun create(initialState: PcpDummyViewState): ReleaseNotesViewModel
    }

    companion object : MavericksViewModelFactory<ReleaseNotesViewModel, PcpDummyViewState> by hiltMavericksViewModelFactory()

    private var selectedPageIndex = 0

    init {
        _viewEvents.post(ReleaseNotesViewEvents.SelectPage(0))
    }

    override fun handle(action: ReleaseNotesAction) {
        when (action) {
            is ReleaseNotesAction.NextPressed -> handleNextPressed(action)
            is ReleaseNotesAction.PageSelected -> handlePageSelected(action)
        }
    }

    private fun handlePageSelected(action: ReleaseNotesAction.PageSelected) {
        selectedPageIndex = action.selectedPageIndex
    }

    private fun handleNextPressed(action: ReleaseNotesAction.NextPressed) {
        if (action.isLastItemSelected) {
            _viewEvents.post(ReleaseNotesViewEvents.Close)
        } else {
            _viewEvents.post(ReleaseNotesViewEvents.SelectPage(++selectedPageIndex))
        }
    }
}
