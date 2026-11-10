/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.labs

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.EmptyViewEvents
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.core.session.clientinfo.DeleteMatrixClientInfoUseCase
import chat.pcp.app.core.session.clientinfo.UpdateMatrixClientInfoUseCase
import kotlinx.coroutines.launch

class PcpSettingsLabsVM @AssistedInject constructor(
        @Assisted initialState: PcpSettingsLabsState,
        private val activeSessionHolder: ActiveSessionHolder,
        private val updateMatrixClientInfoUseCase: UpdateMatrixClientInfoUseCase,
        private val deleteMatrixClientInfoUseCase: DeleteMatrixClientInfoUseCase,
) : PcpViewModel<PcpSettingsLabsState, PcpSettingsLabsAction, EmptyViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<PcpSettingsLabsVM, PcpSettingsLabsState> {
        override fun create(initialState: PcpSettingsLabsState): PcpSettingsLabsVM
    }

    companion object : MavericksViewModelFactory<PcpSettingsLabsVM, PcpSettingsLabsState> by hiltMavericksViewModelFactory()

    override fun handle(action: PcpSettingsLabsAction) {
        when (action) {
            PcpSettingsLabsAction.UpdateClientInfo -> handleUpdateClientInfo()
            PcpSettingsLabsAction.DeleteRecordedClientInfo -> handleDeleteRecordedClientInfo()
        }
    }

    private fun handleUpdateClientInfo() {
        viewModelScope.launch {
            activeSessionHolder.getSafeActiveSession()
                    ?.let { session ->
                        updateMatrixClientInfoUseCase.execute(session)
                    }
        }
    }

    private fun handleDeleteRecordedClientInfo() {
        viewModelScope.launch {
            deleteMatrixClientInfoUseCase.execute()
        }
    }
}
