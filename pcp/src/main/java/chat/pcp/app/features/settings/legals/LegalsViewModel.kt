/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.settings.legals

import com.airbnb.mvrx.Fail
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
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.features.discovery.fetchHomeserverWithTerms
import chat.pcp.app.features.discovery.fetchIdentityServerWithTerms
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.session.Session

class LegalsViewModel @AssistedInject constructor(
        @Assisted initialState: LegalsState,
        private val session: Session,
        private val stringProvider: StringProvider
) : PcpViewModel<LegalsState, LegalsAction, EmptyViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<LegalsViewModel, LegalsState> {
        override fun create(initialState: LegalsState): LegalsViewModel
    }

    companion object : MavericksViewModelFactory<LegalsViewModel, LegalsState> by hiltMavericksViewModelFactory()

    override fun handle(action: LegalsAction) {
        when (action) {
            LegalsAction.Refresh -> loadData()
        }
    }

    private fun loadData() = withState { state ->
        loadHomeserver(state)
        val url = session.identityService().getCurrentIdentityServerUrl()
        if (url.isNullOrEmpty()) {
            setState { copy(hasIdentityServer = false) }
        } else {
            setState { copy(hasIdentityServer = true) }
            loadIdentityServer(state)
        }
    }

    private fun loadHomeserver(state: LegalsState) {
        if (state.homeServer !is Success) {
            setState { copy(homeServer = Loading()) }
            viewModelScope.launch {
                runCatching { session.fetchHomeserverWithTerms(stringProvider.getString(CommonStrings.resources_language)) }
                        .fold(
                                onSuccess = { setState { copy(homeServer = Success(it)) } },
                                onFailure = { setState { copy(homeServer = Fail(it)) } }
                        )
            }
        }
    }

    private fun loadIdentityServer(state: LegalsState) {
        if (state.identityServer !is Success) {
            setState { copy(identityServer = Loading()) }
            viewModelScope.launch {
                runCatching { session.fetchIdentityServerWithTerms(stringProvider.getString(CommonStrings.resources_language)) }
                        .fold(
                                onSuccess = { setState { copy(identityServer = Success(it)) } },
                                onFailure = { setState { copy(identityServer = Fail(it)) } }
                        )
            }
        }
    }
}
