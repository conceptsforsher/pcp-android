/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.rename

import androidx.annotation.VisibleForTesting
import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.features.settings.devices.v2.overview.GetDeviceFullInfoUseCase
import kotlinx.coroutines.flow.firstOrNull
import kotlinx.coroutines.launch

class RenameSessionViewModel @AssistedInject constructor(
        @Assisted val initialState: RenameSessionViewState,
        private val getDeviceFullInfoUseCase: GetDeviceFullInfoUseCase,
        private val renameSessionUseCase: RenameSessionUseCase,
) : PcpViewModel<RenameSessionViewState, RenameSessionAction, RenameSessionViewEvent>(initialState) {

    companion object : MavericksViewModelFactory<RenameSessionViewModel, RenameSessionViewState> by hiltMavericksViewModelFactory()

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<RenameSessionViewModel, RenameSessionViewState> {
        override fun create(initialState: RenameSessionViewState): RenameSessionViewModel
    }

    @VisibleForTesting
    var hasRetrievedOriginalDeviceName = false

    override fun handle(action: RenameSessionAction) {
        when (action) {
            is RenameSessionAction.InitWithLastEditedName -> handleInitWithLastEditedName()
            is RenameSessionAction.EditLocally -> handleEditLocally(action.editedName)
            is RenameSessionAction.SaveModifications -> handleSaveModifications()
        }
    }

    private fun handleInitWithLastEditedName() = withState { state ->
        if (hasRetrievedOriginalDeviceName) {
            postInitEvent()
        } else {
            hasRetrievedOriginalDeviceName = true
            viewModelScope.launch {
                setStateWithOriginalDeviceName(state.deviceId)
                postInitEvent()
            }
        }
    }

    private suspend fun setStateWithOriginalDeviceName(deviceId: String) {
        getDeviceFullInfoUseCase.execute(deviceId)
                .firstOrNull()
                ?.let { deviceFullInfo ->
                    setState { copy(editedDeviceName = deviceFullInfo.deviceInfo.displayName.orEmpty()) }
                }
    }

    private fun postInitEvent() = withState { state ->
        _viewEvents.post(RenameSessionViewEvent.Initialized(state.editedDeviceName))
    }

    private fun handleEditLocally(editedName: String) {
        setState { copy(editedDeviceName = editedName) }
    }

    private fun handleSaveModifications() = withState { viewState ->
        viewModelScope.launch {
            val result = renameSessionUseCase.execute(
                    deviceId = viewState.deviceId,
                    newName = viewState.editedDeviceName,
            )
            val viewEvent = if (result.isSuccess) {
                RenameSessionViewEvent.SessionRenamed
            } else {
                RenameSessionViewEvent.Failure(result.exceptionOrNull() ?: Exception())
            }
            _viewEvents.post(viewEvent)
        }
    }
}
