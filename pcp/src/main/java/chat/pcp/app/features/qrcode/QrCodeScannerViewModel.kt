/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.qrcode

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.PcpDummyViewState
import chat.pcp.app.core.platform.PcpViewModel

class QrCodeScannerViewModel @AssistedInject constructor(
        @Assisted initialState: PcpDummyViewState,
) : PcpViewModel<PcpDummyViewState, QrCodeScannerAction, QrCodeScannerEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<QrCodeScannerViewModel, PcpDummyViewState> {
        override fun create(initialState: PcpDummyViewState): QrCodeScannerViewModel
    }

    companion object : MavericksViewModelFactory<QrCodeScannerViewModel, PcpDummyViewState> by hiltMavericksViewModelFactory()

    override fun handle(action: QrCodeScannerAction) {
        _viewEvents.post(
                when (action) {
                    is QrCodeScannerAction.CodeDecoded -> QrCodeScannerEvents.CodeParsed(action.result, action.isQrCode)
                    is QrCodeScannerAction.SwitchMode -> QrCodeScannerEvents.SwitchMode
                    is QrCodeScannerAction.ScanFailed -> QrCodeScannerEvents.ParseFailed
                }
        )
    }
}
