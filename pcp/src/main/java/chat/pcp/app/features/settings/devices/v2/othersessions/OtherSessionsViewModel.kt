/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.othersessions

import android.content.SharedPreferences
import com.airbnb.mvrx.MavericksViewModelFactory
import com.airbnb.mvrx.Success
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.features.auth.PendingAuthHandler
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.settings.devices.v2.GetDeviceFullInfoListUseCase
import chat.pcp.app.features.settings.devices.v2.RefreshDevicesUseCase
import chat.pcp.app.features.settings.devices.v2.ToggleIpAddressVisibilityUseCase
import chat.pcp.app.features.settings.devices.v2.PcpSessionsList
import chat.pcp.app.features.settings.devices.v2.filter.DeviceManagerFilterType
import chat.pcp.app.features.settings.devices.v2.signout.SignoutSessionsReAuthNeeded
import chat.pcp.app.features.settings.devices.v2.signout.SignoutSessionsUseCase
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.extensions.orFalse
import org.matrix.android.sdk.api.session.uia.DefaultBaseAuth
import timber.log.Timber

class OtherSessionsViewModel @AssistedInject constructor(
        @Assisted private val initialState: OtherSessionsViewState,
        private val activeSessionHolder: ActiveSessionHolder,
        private val getDeviceFullInfoListUseCase: GetDeviceFullInfoListUseCase,
        private val signoutSessionsUseCase: SignoutSessionsUseCase,
        private val pendingAuthHandler: PendingAuthHandler,
        refreshDevicesUseCase: RefreshDevicesUseCase,
        private val pcpPreferences: PcpBasePreferences,
        private val toggleIpAddressVisibilityUseCase: ToggleIpAddressVisibilityUseCase,
) : PcpSessionsList<OtherSessionsViewState, OtherSessionsAction, OtherSessionsViewEvents>(
        initialState, activeSessionHolder, refreshDevicesUseCase
), SharedPreferences.OnSharedPreferenceChangeListener {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<OtherSessionsViewModel, OtherSessionsViewState> {
        override fun create(initialState: OtherSessionsViewState): OtherSessionsViewModel
    }

    companion object : MavericksViewModelFactory<OtherSessionsViewModel, OtherSessionsViewState> by hiltMavericksViewModelFactory()

    private var observeDevicesJob: Job? = null

    init {
        observeDevices(initialState.currentFilter)
        refreshIpAddressVisibility()
        observePreferences()
        initDelegatedOidcAuthEnabled()
    }

    private fun initDelegatedOidcAuthEnabled() {
        setState {
            copy(
                    delegatedOidcAuthEnabled = activeSessionHolder.getSafeActiveSession()
                            ?.homeServerCapabilitiesService()
                            ?.getHomeServerCapabilities()
                            ?.delegatedOidcAuthEnabled
                            .orFalse()
            )
        }
    }

    override fun onSharedPreferenceChanged(sharedPreferences: SharedPreferences?, key: String?) {
        refreshIpAddressVisibility()
    }

    private fun observePreferences() {
        pcpPreferences.subscribeToChanges(this)
    }

    override fun onCleared() {
        pcpPreferences.unsubscribeToChanges(this)
        super.onCleared()
    }

    private fun refreshIpAddressVisibility() {
        val shouldShowIpAddress = pcpPreferences.showIpAddressInSessionManagerScreens()
        setState {
            copy(isShowingIpAddress = shouldShowIpAddress)
        }
    }

    private fun observeDevices(currentFilter: DeviceManagerFilterType) {
        observeDevicesJob?.cancel()
        observeDevicesJob = getDeviceFullInfoListUseCase.execute(
                filterType = currentFilter,
                excludeCurrentDevice = initialState.excludeCurrentDevice
        )
                .execute { async ->
                    copy(
                            devices = async,
                    )
                }
    }

    override fun handle(action: OtherSessionsAction) {
        when (action) {
            is OtherSessionsAction.PasswordAuthDone -> handlePasswordAuthDone(action)
            OtherSessionsAction.ReAuthCancelled -> handleReAuthCancelled()
            OtherSessionsAction.SsoAuthDone -> handleSsoAuthDone()
            is OtherSessionsAction.FilterDevices -> handleFilterDevices(action)
            OtherSessionsAction.DisableSelectMode -> handleDisableSelectMode()
            is OtherSessionsAction.EnableSelectMode -> handleEnableSelectMode(action.deviceId)
            is OtherSessionsAction.ToggleSelectionForDevice -> handleToggleSelectionForDevice(action.deviceId)
            OtherSessionsAction.DeselectAll -> handleDeselectAll()
            OtherSessionsAction.SelectAll -> handleSelectAll()
            OtherSessionsAction.MultiSignout -> handleMultiSignout()
            OtherSessionsAction.ToggleIpAddressVisibility -> handleToggleIpAddressVisibility()
        }
    }

    private fun handleToggleIpAddressVisibility() {
        toggleIpAddressVisibilityUseCase.execute()
    }

    private fun handleFilterDevices(action: OtherSessionsAction.FilterDevices) {
        setState {
            copy(
                    currentFilter = action.filterType
            )
        }
        observeDevices(action.filterType)
    }

    private fun handleDisableSelectMode() {
        setSelectionForAllDevices(isSelected = false, enableSelectMode = false)
    }

    private fun handleEnableSelectMode(deviceId: String?) {
        toggleSelectionForDevice(deviceId, enableSelectMode = true)
    }

    private fun handleToggleSelectionForDevice(deviceId: String) = withState { state ->
        toggleSelectionForDevice(deviceId, enableSelectMode = state.isSelectModeEnabled)
    }

    private fun toggleSelectionForDevice(deviceId: String?, enableSelectMode: Boolean) = withState { state ->
        val updatedDevices = if (state.devices is Success) {
            val devices = state.devices.invoke().toMutableList()
            val indexToUpdate = devices.indexOfFirst { it.deviceInfo.deviceId == deviceId }
            if (indexToUpdate >= 0) {
                val currentInfo = devices[indexToUpdate]
                val updatedInfo = currentInfo.copy(isSelected = !currentInfo.isSelected)
                devices[indexToUpdate] = updatedInfo
            }
            Success(devices)
        } else {
            state.devices
        }

        setState {
            copy(
                    devices = updatedDevices,
                    isSelectModeEnabled = enableSelectMode
            )
        }
    }

    private fun handleSelectAll() = withState { state ->
        setSelectionForAllDevices(isSelected = true, enableSelectMode = state.isSelectModeEnabled)
    }

    private fun handleDeselectAll() = withState { state ->
        setSelectionForAllDevices(isSelected = false, enableSelectMode = state.isSelectModeEnabled)
    }

    private fun setSelectionForAllDevices(isSelected: Boolean, enableSelectMode: Boolean) = withState { state ->
        val updatedDevices = if (state.devices is Success) {
            val updatedDevices = state.devices.invoke().map { it.copy(isSelected = isSelected) }
            Success(updatedDevices)
        } else {
            state.devices
        }

        setState {
            copy(
                    devices = updatedDevices,
                    isSelectModeEnabled = enableSelectMode
            )
        }
    }

    private fun handleMultiSignout() = withState { state ->
        viewModelScope.launch {
            setLoading(true)
            val deviceIds = getDeviceIdsToSignout(state)
            if (deviceIds.isEmpty()) {
                return@launch
            }
            val result = signout(deviceIds)
            setLoading(false)

            val error = result.exceptionOrNull()
            if (error == null) {
                onSignoutSuccess()
            } else {
                onSignoutFailure(error)
            }
        }
    }

    private fun getDeviceIdsToSignout(state: OtherSessionsViewState): List<String> {
        return if (state.isSelectModeEnabled) {
            state.devices()?.filter { it.isSelected }.orEmpty()
        } else {
            state.devices().orEmpty()
        }.mapNotNull { it.deviceInfo.deviceId }
    }

    private suspend fun signout(deviceIds: List<String>) = signoutSessionsUseCase.execute(deviceIds, this::onReAuthNeeded)

    private fun onReAuthNeeded(reAuthNeeded: SignoutSessionsReAuthNeeded) {
        Timber.d("onReAuthNeeded")
        pendingAuthHandler.pendingAuth = DefaultBaseAuth(session = reAuthNeeded.flowResponse.session)
        pendingAuthHandler.uiaContinuation = reAuthNeeded.uiaContinuation
        _viewEvents.post(OtherSessionsViewEvents.RequestReAuth(reAuthNeeded.flowResponse, reAuthNeeded.errCode))
    }

    private fun setLoading(isLoading: Boolean) {
        setState { copy(isLoading = isLoading) }
    }

    private fun onSignoutSuccess() {
        Timber.d("signout success")
        refreshDeviceList()
        _viewEvents.post(OtherSessionsViewEvents.SignoutSuccess)
    }

    private fun onSignoutFailure(failure: Throwable) {
        Timber.e("signout failure", failure)
        _viewEvents.post(OtherSessionsViewEvents.SignoutError(failure))
    }

    private fun handleSsoAuthDone() {
        pendingAuthHandler.ssoAuthDone()
    }

    private fun handlePasswordAuthDone(action: OtherSessionsAction.PasswordAuthDone) {
        pendingAuthHandler.passwordAuthDone(action.password)
    }

    private fun handleReAuthCancelled() {
        pendingAuthHandler.reAuthCancelled()
    }
}
