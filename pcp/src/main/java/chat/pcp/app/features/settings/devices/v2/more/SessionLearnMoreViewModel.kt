/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.more

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.EmptyAction
import chat.pcp.app.core.platform.EmptyViewEvents
import chat.pcp.app.core.platform.PcpViewModel

class SessionLearnMoreViewModel @AssistedInject constructor(
        @Assisted initialState: SessionLearnMoreViewState,
) : PcpViewModel<SessionLearnMoreViewState, EmptyAction, EmptyViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<SessionLearnMoreViewModel, SessionLearnMoreViewState> {
        override fun create(initialState: SessionLearnMoreViewState): SessionLearnMoreViewModel
    }

    companion object : MavericksViewModelFactory<SessionLearnMoreViewModel, SessionLearnMoreViewState> by hiltMavericksViewModelFactory()

    override fun handle(action: EmptyAction) {
        // do nothing
    }
}
