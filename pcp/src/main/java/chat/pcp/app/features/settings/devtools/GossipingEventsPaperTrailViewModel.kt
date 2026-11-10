/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devtools

import androidx.lifecycle.asFlow
import androidx.paging.PagedList
import com.airbnb.mvrx.Async
import com.airbnb.mvrx.Loading
import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.MavericksViewModelFactory
import com.airbnb.mvrx.Uninitialized
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.EmptyAction
import chat.pcp.app.core.platform.EmptyViewEvents
import chat.pcp.app.core.platform.PcpViewModel
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.crypto.model.AuditTrail

data class GossipingEventsPaperTrailState(
        val events: Async<PagedList<AuditTrail>> = Uninitialized
) : MavericksState

class GossipingEventsPaperTrailViewModel @AssistedInject constructor(
        @Assisted initialState: GossipingEventsPaperTrailState,
        private val session: Session
) :
        PcpViewModel<GossipingEventsPaperTrailState, EmptyAction, EmptyViewEvents>(initialState) {

    init {
        refresh()
    }

    fun refresh() {
        setState {
            copy(events = Loading())
        }
        if (session.cryptoService().supportKeyRequestInspection()) {
            session.cryptoService().getGossipingEventsTrail()
                    .asFlow()
                    .execute {
                        copy(events = it)
                    }
        }
    }

    override fun handle(action: EmptyAction) {}

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<GossipingEventsPaperTrailViewModel, GossipingEventsPaperTrailState> {
        override fun create(initialState: GossipingEventsPaperTrailState): GossipingEventsPaperTrailViewModel
    }

    companion object : MavericksViewModelFactory<GossipingEventsPaperTrailViewModel, GossipingEventsPaperTrailState> by hiltMavericksViewModelFactory()
}
