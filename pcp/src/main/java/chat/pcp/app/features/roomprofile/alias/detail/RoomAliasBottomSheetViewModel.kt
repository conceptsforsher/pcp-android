/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.roomprofile.alias.detail

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.pcp.app.core.di.MavericksAssistedViewModelFactory
import chat.pcp.app.core.di.hiltMavericksViewModelFactory
import chat.pcp.app.core.platform.EmptyAction
import chat.pcp.app.core.platform.EmptyViewEvents
import chat.pcp.app.core.platform.PcpViewModel
import org.matrix.android.sdk.api.session.Session

class RoomAliasBottomSheetViewModel @AssistedInject constructor(
        @Assisted initialState: RoomAliasBottomSheetState,
        session: Session
) : PcpViewModel<RoomAliasBottomSheetState, EmptyAction, EmptyViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<RoomAliasBottomSheetViewModel, RoomAliasBottomSheetState> {
        override fun create(initialState: RoomAliasBottomSheetState): RoomAliasBottomSheetViewModel
    }

    companion object : MavericksViewModelFactory<RoomAliasBottomSheetViewModel, RoomAliasBottomSheetState> by hiltMavericksViewModelFactory()

    init {
        setState {
            copy(
                    matrixToLink = session.permalinkService().createPermalink(alias)
            )
        }
    }

    override fun handle(action: EmptyAction) {
        // No op
    }
}
