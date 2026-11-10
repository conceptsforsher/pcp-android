/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.locale

import com.airbnb.mvrx.MavericksViewModelFactory
import com.airbnb.mvrx.Success
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.features.configuration.PcpConfiguration
import chat.pcp.app.features.settings.PcpLocale
import kotlinx.coroutines.launch

class LocalePickerViewModel @AssistedInject constructor(
        @Assisted initialState: LocalePickerViewState,
        private val vectorConfiguration: PcpConfiguration,
        private val vectorLocale: PcpLocale,
) : PcpViewModel<LocalePickerViewState, LocalePickerAction, LocalePickerViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<LocalePickerViewModel, LocalePickerViewState> {
        override fun create(initialState: LocalePickerViewState): LocalePickerViewModel
    }

    init {
        setState {
            copy(
                    currentLocale = vectorLocale.applicationLocale
            )
        }
        viewModelScope.launch {
            val result = vectorLocale.getSupportedLocales()

            setState {
                copy(
                        locales = Success(result)
                )
            }
        }
    }

    companion object : MavericksViewModelFactory<LocalePickerViewModel, LocalePickerViewState> by hiltMavericksViewModelFactory()

    override fun handle(action: LocalePickerAction) {
        when (action) {
            is LocalePickerAction.SelectLocale -> handleSelectLocale(action)
        }
    }

    private fun handleSelectLocale(action: LocalePickerAction.SelectLocale) {
        vectorLocale.saveApplicationLocale(action.locale)
        vectorConfiguration.applyToApplicationContext()
        _viewEvents.post(LocalePickerViewEvents.RestartActivity)
    }
}
