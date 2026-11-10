/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2

import android.content.SharedPreferences
import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.features.auth.PendingAuthHandler
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.settings.devices.v2.filter.DeviceManagerFilterType
import chat.pcp.app.features.settings.devices.v2.signout.SignoutSessionsReAuthNeeded
import chat.pcp.app.features.settings.devices.v2.signout.SignoutSessionsUseCase
import chat.pcp.app.features.settings.devices.v2.verification.CheckIfCurrentSessionCanBeVerifiedUseCase
import chat.pcp.app.features.settings.devices.v2.verification.GetCurrentSessionCrossSigningInfoUseCase
import kotlinx.coroutines.flow.combine
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.extensions.orFalse
import org.matrix.android.sdk.api.session.uia.DefaultBaseAuth
import timber.log.Timber

class DevicesViewModel @AssistedInject constructor(
        @Assisted initialState: DevicesViewState,
        private val activeSessionHolder: ActiveSessionHolder,
        private val getCurrentSessionCrossSigningInfoUseCase: GetCurrentSessionCrossSigningInfoUseCase,
        private val getDeviceFullInfoListUseCase: GetDeviceFullInfoListUseCase,
        private val refreshDevicesOnCryptoDevicesChangeUseCase: RefreshDevicesOnCryptoDevicesChangeUseCase,
        private val checkIfCurrentSessionCanBeVerifiedUseCase: CheckIfCurrentSessionCanBeVerifiedUseCase,
        private val signoutSessionsUseCase: SignoutSessionsUseCase,
        private val pendingAuthHandler: PendingAuthHandler,
        refreshDevicesUseCase: RefreshDevicesUseCase,
        private val pcpPreferences: PcpBasePreferences,
        private val toggleIpAddressVisibilityUseCase: ToggleIpAddressVisibilityUseCase,
) : PcpSessionsList<DevicesViewState,
        DevicesAction,
        DevicesViewEvent>(initialState, activeSessionHolder, refreshDevicesUseCase),
        SharedPreferences.OnSharedPreferenceChangeListener {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<DevicesViewModel, DevicesViewState> {
        override fun create(initialState: DevicesViewState): DevicesViewModel
    }

    companion object : MavericksViewModelFactory<DevicesViewModel, DevicesViewState> by hiltMavericksViewModelFactory()

    init {
        observeCurrentSessionCrossSigningInfo()
        observeDevices()
        refreshDevicesOnCryptoDevicesChange()
        refreshDeviceList()
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

    private fun observeCurrentSessionCrossSigningInfo() {
        getCurrentSessionCrossSigningInfoUseCase.execute()
                .onEach { crossSigningInfo ->
                    setState {
                        copy(currentSessionCrossSigningInfo = crossSigningInfo)
                    }
                }
                .launchIn(viewModelScope)
    }

    private fun observeDevices() {
        val allSessionsFlow = getDeviceFullInfoListUseCase.execute(
                filterType = DeviceManagerFilterType.ALL_SESSIONS,
                excludeCurrentDevice = false,
        )
        val unverifiedSessionsFlow = getDeviceFullInfoListUseCase.execute(
                filterType = DeviceManagerFilterType.UNVERIFIED,
                excludeCurrentDevice = true,
        )
        val inactiveSessionsFlow = getDeviceFullInfoListUseCase.execute(
                filterType = DeviceManagerFilterType.INACTIVE,
                excludeCurrentDevice = true,
        )

        combine(allSessionsFlow, unverifiedSessionsFlow, inactiveSessionsFlow) { allSessions, unverifiedSessions, inactiveSessions ->
            DeviceFullInfoList(
                    allSessions = allSessions,
                    unverifiedSessionsCount = unverifiedSessions.size,
                    inactiveSessionsCount = inactiveSessions.size,
            )
        }
                .execute { async -> copy(devices = async) }
    }

    private fun refreshDevicesOnCryptoDevicesChange() {
        viewModelScope.launch {
            refreshDevicesOnCryptoDevicesChangeUseCase.execute()
        }
    }

    override fun handle(action: DevicesAction) {
        when (action) {
            is DevicesAction.PasswordAuthDone -> handlePasswordAuthDone(action)
            DevicesAction.ReAuthCancelled -> handleReAuthCancelled()
            DevicesAction.SsoAuthDone -> handleSsoAuthDone()
            is DevicesAction.VerifyCurrentSession -> handleVerifyCurrentSessionAction()
            is DevicesAction.MarkAsManuallyVerified -> handleMarkAsManuallyVerifiedAction()
            DevicesAction.MultiSignoutOtherSessions -> handleMultiSignoutOtherSessions()
            DevicesAction.ToggleIpAddressVisibility -> handleToggleIpAddressVisibility()
        }
    }

    private fun handleToggleIpAddressVisibility() {
        toggleIpAddressVisibilityUseCase.execute()
    }

    private fun handleVerifyCurrentSessionAction() {
        viewModelScope.launch {
            val currentSessionCanBeVerified = checkIfCurrentSessionCanBeVerifiedUseCase.execute()
            if (currentSessionCanBeVerified) {
                _viewEvents.post(DevicesViewEvent.SelfVerification)
            } else {
                _viewEvents.post(DevicesViewEvent.PromptResetSecrets)
            }
        }
    }

    private fun handleMarkAsManuallyVerifiedAction() {
        // TODO implement when needed
    }

    private fun handleMultiSignoutOtherSessions() = withState { state ->
        viewModelScope.launch {
            setLoading(true)
            val deviceIds = getDeviceIdsOfOtherSessions(state)
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

    private fun getDeviceIdsOfOtherSessions(state: DevicesViewState): List<String> {
        val currentDeviceId = state.currentSessionCrossSigningInfo.deviceId
        return state.devices()
                ?.allSessions
                ?.mapNotNull { fullInfo -> fullInfo.deviceInfo.deviceId.takeUnless { it == currentDeviceId } }
                .orEmpty()
    }

    private suspend fun signout(deviceIds: List<String>) = signoutSessionsUseCase.execute(deviceIds, this::onReAuthNeeded)

    private fun onReAuthNeeded(reAuthNeeded: SignoutSessionsReAuthNeeded) {
        Timber.d("onReAuthNeeded")
        pendingAuthHandler.pendingAuth = DefaultBaseAuth(session = reAuthNeeded.flowResponse.session)
        pendingAuthHandler.uiaContinuation = reAuthNeeded.uiaContinuation
        _viewEvents.post(DevicesViewEvent.RequestReAuth(reAuthNeeded.flowResponse, reAuthNeeded.errCode))
    }

    private fun setLoading(isLoading: Boolean) {
        setState { copy(isLoading = isLoading) }
    }

    private fun onSignoutSuccess() {
        Timber.d("signout success")
        refreshDeviceList()
        _viewEvents.post(DevicesViewEvent.SignoutSuccess)
    }

    private fun onSignoutFailure(failure: Throwable) {
        Timber.e("signout failure", failure)
        _viewEvents.post(DevicesViewEvent.SignoutError(failure))
    }

    private fun handleSsoAuthDone() {
        pendingAuthHandler.ssoAuthDone()
    }

    private fun handlePasswordAuthDone(action: DevicesAction.PasswordAuthDone) {
        pendingAuthHandler.passwordAuthDone(action.password)
    }

    private fun handleReAuthCancelled() {
        pendingAuthHandler.reAuthCancelled()
    }
}
