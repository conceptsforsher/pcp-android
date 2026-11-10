/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.breadcrumbs

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.EmptyAction
import chat.pcp.app.core.platform.EmptyViewEvents
import chat.pcp.app.core.platform.PcpViewModel
import org.matrix.android.sdk.api.query.QueryStringValue
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.room.model.Membership
import org.matrix.android.sdk.api.session.room.roomSummaryQueryParams
import org.matrix.android.sdk.flow.flow

class BreadcrumbsViewModel @AssistedInject constructor(
        @Assisted initialState: BreadcrumbsViewState,
        private val session: Session
) :
        PcpViewModel<BreadcrumbsViewState, EmptyAction, EmptyViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<BreadcrumbsViewModel, BreadcrumbsViewState> {
        override fun create(initialState: BreadcrumbsViewState): BreadcrumbsViewModel
    }

    companion object : MavericksViewModelFactory<BreadcrumbsViewModel, BreadcrumbsViewState> by hiltMavericksViewModelFactory()

    init {
        observeBreadcrumbs()
    }

    override fun handle(action: EmptyAction) {
        // No op
    }

    // PRIVATE METHODS *****************************************************************************

    private fun observeBreadcrumbs() {
        session.flow()
                .liveBreadcrumbs(roomSummaryQueryParams {
                    displayName = QueryStringValue.NoCondition
                    memberships = listOf(Membership.JOIN)
                })
                .execute { asyncBreadcrumbs ->
                    copy(asyncBreadcrumbs = asyncBreadcrumbs)
                }
    }
}
