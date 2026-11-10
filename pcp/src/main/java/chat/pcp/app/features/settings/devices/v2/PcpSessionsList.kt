/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2

import com.airbnb.mvrx.MavericksState
import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.core.platform.PcpViewEvents
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.core.platform.PcpViewModelAction
import chat.pcp.app.core.utils.PublishDataSource
import chat.pcp.lib.core.utils.flow.throttleFirst
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach
import org.matrix.android.sdk.api.session.crypto.verification.VerificationEvent
import org.matrix.android.sdk.api.session.crypto.verification.VerificationService
import org.matrix.android.sdk.api.session.crypto.verification.VerificationTransaction
import kotlin.time.Duration.Companion.seconds

abstract class PcpSessionsList<S : MavericksState, VA : PcpViewModelAction, VE : PcpViewEvents>(
        initialState: S,
        private val activeSessionHolder: ActiveSessionHolder,
        private val refreshDevicesUseCase: RefreshDevicesUseCase,
) : PcpViewModel<S, VA, VE>(initialState), VerificationService.Listener {

    private val refreshSource = PublishDataSource<Unit>()
    private val refreshThrottleDelayMs = 4.seconds.inWholeMilliseconds

    init {
//        addVerificationListener()
        observeRefreshSource()
        activeSessionHolder.getSafeActiveSession()
                ?.cryptoService()
                ?.verificationService()
                ?.requestEventFlow()
                ?.onEach {
                    when (it) {
                        is VerificationEvent.RequestAdded -> verificationRequestCreated(it.request)
                        is VerificationEvent.RequestUpdated -> verificationRequestUpdated(it.request)
                        is VerificationEvent.TransactionAdded -> transactionCreated(it.transaction)
                        is VerificationEvent.TransactionUpdated -> transactionUpdated(it.transaction)
                    }
                }
                ?.launchIn(viewModelScope)
    }

//    override fun onCleared() {
//        super.onCleared()
//    }

//    private fun addVerificationListener() {
//        activeSessionHolder.getSafeActiveSession()
//                ?.cryptoService()
//                ?.verificationService()
//                ?.addListener(this)
//    }

//    private fun removeVerificationListener() {
//        activeSessionHolder.getSafeActiveSession()
//                ?.cryptoService()
//                ?.verificationService()
//                ?.removeListener(this)
//    }

    private fun observeRefreshSource() {
        refreshSource.stream()
                .throttleFirst(refreshThrottleDelayMs)
                .onEach { refreshDevicesUseCase.execute() }
                .launchIn(viewModelScope)
    }

    override fun transactionUpdated(tx: VerificationTransaction) {
        if (tx.isSuccessful()) {
            refreshDeviceList()
        }
    }

    /**
     * Force the refresh of the devices list.
     * The devices list is the list of the devices where the user is logged in.
     * It can be any mobile devices, and any browsers.
     */
    fun refreshDeviceList() {
        refreshSource.post(Unit)
    }
}
