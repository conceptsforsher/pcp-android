/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.attachments

import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.EmptyViewEvents
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.core.platform.PcpViewModelAction
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.settings.PcpBasePreferences

class AttachmentTypeSelectorViewModel @AssistedInject constructor(
        @Assisted initialState: AttachmentTypeSelectorViewState,
        private val vectorFeatures: PcpFeatures,
        private val pcpPreferences: PcpBasePreferences,
) : PcpViewModel<AttachmentTypeSelectorViewState, AttachmentTypeSelectorAction, EmptyViewEvents>(initialState) {
    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<AttachmentTypeSelectorViewModel, AttachmentTypeSelectorViewState> {
        override fun create(initialState: AttachmentTypeSelectorViewState): AttachmentTypeSelectorViewModel
    }

    companion object : MavericksViewModelFactory<AttachmentTypeSelectorViewModel, AttachmentTypeSelectorViewState> by hiltMavericksViewModelFactory()

    override fun handle(action: AttachmentTypeSelectorAction) = when (action) {
        is AttachmentTypeSelectorAction.ToggleTextFormatting -> setTextFormattingEnabled(action.isEnabled)
    }

    init {
        setState {
            copy(
                    isLocationVisible = vectorFeatures.isLocationSharingEnabled(),
                    isVoiceBroadcastVisible = vectorFeatures.isVoiceBroadcastEnabled(),
                    isTextFormattingEnabled = pcpPreferences.isTextFormattingEnabled(),
            )
        }
    }

    private fun setTextFormattingEnabled(isEnabled: Boolean) {
        pcpPreferences.setTextFormattingEnabled(isEnabled)
        setState {
            copy(
                    isTextFormattingEnabled = isEnabled
            )
        }
    }
}

data class AttachmentTypeSelectorViewState(
        val isLocationVisible: Boolean = false,
        val isVoiceBroadcastVisible: Boolean = false,
        val isTextFormattingEnabled: Boolean = false,
) : MavericksState

sealed interface AttachmentTypeSelectorAction : PcpViewModelAction {
    data class ToggleTextFormatting(val isEnabled: Boolean) : AttachmentTypeSelectorAction
}
