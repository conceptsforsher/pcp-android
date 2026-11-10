/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.media

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.PcpDummyViewState
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.features.media.domain.usecase.DownloadMediaUseCase
import chat.pcp.app.features.session.coroutineScope
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.session.Session

class PcpAttachmentViewModel @AssistedInject constructor(
        @Assisted initialState: PcpDummyViewState,
        private val session: Session,
        private val downloadMediaUseCase: DownloadMediaUseCase
) : PcpViewModel<PcpDummyViewState, PcpAttachmentAction, PcpAttachmentEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<PcpAttachmentViewModel, PcpDummyViewState> {
        override fun create(initialState: PcpDummyViewState): PcpAttachmentViewModel
    }

    companion object : MavericksViewModelFactory<PcpAttachmentViewModel, PcpDummyViewState> by hiltMavericksViewModelFactory()

    var pendingAction: PcpAttachmentAction? = null

    override fun handle(action: PcpAttachmentAction) {
        when (action) {
            is PcpAttachmentAction.DownloadMedia -> handleDownloadAction(action)
        }
    }

    private fun handleDownloadAction(action: PcpAttachmentAction.DownloadMedia) {
        // launch in the coroutine scope session to avoid binding the coroutine to the lifecycle of the VM
        session.coroutineScope.launch {
            // Success event is handled via a notification inside the use case
            downloadMediaUseCase.execute(action.file)
                    .onFailure { _viewEvents.post(PcpAttachmentEvents.ErrorDownloadingMedia(it)) }
        }
    }
}
