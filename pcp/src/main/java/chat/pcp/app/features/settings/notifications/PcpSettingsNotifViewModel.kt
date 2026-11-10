/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.notifications

import android.content.SharedPreferences
import androidx.annotation.VisibleForTesting
import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.PcpDummyViewState
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.core.pushers.EnsureFcmTokenIsRetrievedUseCase
import chat.pcp.app.core.pushers.PushersManager
import chat.pcp.app.core.pushers.RegisterUnifiedPushUseCase
import chat.pcp.app.core.pushers.UnregisterUnifiedPushUseCase
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.app.features.settings.notifications.usecase.DisableNotificationsForCurrentSessionUseCase
import chat.pcp.app.features.settings.notifications.usecase.EnableNotificationsForCurrentSessionUseCase
import chat.pcp.app.features.settings.notifications.usecase.ToggleNotificationsForCurrentSessionUseCase
import kotlinx.coroutines.launch

class PcpSettingsNotifViewModel @AssistedInject constructor(
        @Assisted initialState: PcpDummyViewState,
        private val pushersManager: PushersManager,
        private val pcpPreferences: PcpBasePreferences,
        private val enableNotificationsForCurrentSessionUseCase: EnableNotificationsForCurrentSessionUseCase,
        private val disableNotificationsForCurrentSessionUseCase: DisableNotificationsForCurrentSessionUseCase,
        private val unregisterUnifiedPushUseCase: UnregisterUnifiedPushUseCase,
        private val registerUnifiedPushUseCase: RegisterUnifiedPushUseCase,
        private val ensureFcmTokenIsRetrievedUseCase: EnsureFcmTokenIsRetrievedUseCase,
        private val toggleNotificationsForCurrentSessionUseCase: ToggleNotificationsForCurrentSessionUseCase,
) : PcpViewModel<PcpDummyViewState, PcpSettingsNotifAction, PcpSettingsNotifEvent>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<PcpSettingsNotifViewModel, PcpDummyViewState> {
        override fun create(initialState: PcpDummyViewState): PcpSettingsNotifViewModel
    }

    companion object : MavericksViewModelFactory<PcpSettingsNotifViewModel, PcpDummyViewState> by hiltMavericksViewModelFactory()

    @VisibleForTesting
    val notificationsPreferenceListener: SharedPreferences.OnSharedPreferenceChangeListener =
            SharedPreferences.OnSharedPreferenceChangeListener { _, key ->
                if (key == PcpBasePreferences.SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY) {
                    if (pcpPreferences.areNotificationEnabledForDevice()) {
                        _viewEvents.post(PcpSettingsNotifEvent.NotificationsForDeviceEnabled)
                    } else {
                        _viewEvents.post(PcpSettingsNotifEvent.NotificationsForDeviceDisabled)
                    }
                }
            }

    init {
        observeNotificationsEnabledPreference()
    }

    private fun observeNotificationsEnabledPreference() {
        pcpPreferences.subscribeToChanges(notificationsPreferenceListener)
    }

    override fun onCleared() {
        pcpPreferences.unsubscribeToChanges(notificationsPreferenceListener)
        super.onCleared()
    }

    override fun handle(action: PcpSettingsNotifAction) {
        when (action) {
            PcpSettingsNotifAction.DisableNotificationsForDevice -> handleDisableNotificationsForDevice()
            is PcpSettingsNotifAction.EnableNotificationsForDevice -> handleEnableNotificationsForDevice(action.pushDistributor)
            is PcpSettingsNotifAction.RegisterPushDistributor -> handleRegisterPushDistributor(action.pushDistributor)
        }
    }

    private fun handleDisableNotificationsForDevice() {
        viewModelScope.launch {
            disableNotificationsForCurrentSessionUseCase.execute()
            _viewEvents.post(PcpSettingsNotifEvent.NotificationsForDeviceDisabled)
        }
    }

    private fun handleEnableNotificationsForDevice(distributor: String) {
        viewModelScope.launch {
            when (enableNotificationsForCurrentSessionUseCase.execute(distributor)) {
                is EnableNotificationsForCurrentSessionUseCase.EnableNotificationsResult.NeedToAskUserForDistributor -> {
                    _viewEvents.post(PcpSettingsNotifEvent.AskUserForPushDistributor)
                }
                EnableNotificationsForCurrentSessionUseCase.EnableNotificationsResult.Success -> {
                    _viewEvents.post(PcpSettingsNotifEvent.NotificationsForDeviceEnabled)
                }
            }
        }
    }

    private fun handleRegisterPushDistributor(distributor: String) {
        viewModelScope.launch {
            unregisterUnifiedPushUseCase.execute(pushersManager)
            when (registerUnifiedPushUseCase.execute(distributor)) {
                RegisterUnifiedPushUseCase.RegisterUnifiedPushResult.NeedToAskUserForDistributor -> {
                    _viewEvents.post(PcpSettingsNotifEvent.AskUserForPushDistributor)
                }
                RegisterUnifiedPushUseCase.RegisterUnifiedPushResult.Success -> {
                    val areNotificationsEnabled = pcpPreferences.areNotificationEnabledForDevice()
                    ensureFcmTokenIsRetrievedUseCase.execute(pushersManager, registerPusher = areNotificationsEnabled)
                    toggleNotificationsForCurrentSessionUseCase.execute(enabled = areNotificationsEnabled)
                    _viewEvents.post(PcpSettingsNotifEvent.NotificationMethodChanged)
                }
            }
        }
    }
}
