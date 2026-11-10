/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.rageshake

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.EmptyAction
import chat.pcp.app.core.platform.EmptyViewEvents
import chat.pcp.app.core.platform.PcpViewModel
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.extensions.tryOrNull

class BugReportViewModel @AssistedInject constructor(
        @Assisted initialState: BugReportState,
        val activeSessionHolder: ActiveSessionHolder
) : PcpViewModel<BugReportState, EmptyAction, EmptyViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<BugReportViewModel, BugReportState> {
        override fun create(initialState: BugReportState): BugReportViewModel
    }

    companion object : MavericksViewModelFactory<BugReportViewModel, BugReportState> by hiltMavericksViewModelFactory()

    init {
        fetchHomeserverVersion()
    }

    private fun fetchHomeserverVersion() {
        viewModelScope.launch {
            val version = tryOrNull {
                activeSessionHolder.getSafeActiveSession()
                        ?.federationService()
                        ?.getFederationVersion()
                        ?.let { "${it.name} - ${it.version}" }
            } ?: "undefined"

            setState {
                copy(
                        serverVersion = version
                )
            }
        }
    }

    override fun handle(action: EmptyAction) {
        // No op
    }
}
