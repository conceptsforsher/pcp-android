/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.analytics.accountdata

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.EmptyAction
import chat.pcp.app.core.platform.EmptyViewEvents
import chat.pcp.app.core.platform.PcpDummyViewState
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.features.analytics.PcpAnalytics
import chat.pcp.app.features.analytics.log.analyticsTag
import kotlinx.coroutines.flow.combine
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.mapNotNull
import kotlinx.coroutines.flow.onEach
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.events.model.toContent
import org.matrix.android.sdk.api.session.events.model.toModel
import org.matrix.android.sdk.api.session.sync.SyncRequestState
import org.matrix.android.sdk.flow.flow
import timber.log.Timber
import java.util.UUID

class AnalyticsAccountDataViewModel @AssistedInject constructor(
        @Assisted initialState: PcpDummyViewState,
        private val session: Session,
        private val analytics: PcpAnalytics
) : PcpViewModel<PcpDummyViewState, EmptyAction, EmptyViewEvents>(initialState) {

    private var checkDone: Boolean = false

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<AnalyticsAccountDataViewModel, PcpDummyViewState> {
        override fun create(initialState: PcpDummyViewState): AnalyticsAccountDataViewModel
    }

    companion object : MavericksViewModelFactory<AnalyticsAccountDataViewModel, PcpDummyViewState> by hiltMavericksViewModelFactory() {
        private const val ANALYTICS_EVENT_TYPE = "chat.pcp.analytics"
    }

    init {
        observeAccountData()
        observeInitSync()
    }

    private fun observeInitSync() {
        combine(
                session.syncService().getSyncRequestStateFlow(),
                analytics.getUserConsent(),
                analytics.getAnalyticsId()
        ) { status, userConsent, analyticsId ->
            if (status is SyncRequestState.IncrementalSyncIdle &&
                    userConsent &&
                    analyticsId.isEmpty() &&
                    !checkDone) {
                // Initial sync is over, analytics Id from account data is missing and user has given consent to use analytics
                checkDone = true
                createAnalyticsAccountData()
            }
        }.launchIn(viewModelScope)
    }

    private fun observeAccountData() {
        session.flow()
                .liveUserAccountData(setOf(ANALYTICS_EVENT_TYPE))
                .mapNotNull { it.firstOrNull() }
                .mapNotNull { it.content.toModel<AnalyticsAccountDataContent>() }
                .onEach { analyticsAccountDataContent ->
                    if (analyticsAccountDataContent.id.isNullOrEmpty()) {
                        // Probably consent revoked from Element Web
                        // Ignore here
                        Timber.tag(analyticsTag.value).d("Consent revoked from Element Web?")
                    } else {
                        Timber.tag(analyticsTag.value).d("AnalyticsId has been retrieved")
                        analytics.setAnalyticsId(analyticsAccountDataContent.id)
                    }
                }
                .launchIn(viewModelScope)
    }

    override fun handle(action: EmptyAction) {
        // No op
    }

    private fun createAnalyticsAccountData() {
        val content = AnalyticsAccountDataContent(
                id = UUID.randomUUID().toString()
        )

        viewModelScope.launch {
            session.accountDataService().updateUserAccountData(ANALYTICS_EVENT_TYPE, content.toContent())
        }
    }
}
