/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.manage

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.model.RoomSummary

sealed class SpaceAddRoomActions : PcpViewModelAction {
    data class UpdateFilter(val filter: String) : SpaceAddRoomActions()
    data class ToggleSelection(val roomSummary: RoomSummary) : SpaceAddRoomActions()
    object Save : SpaceAddRoomActions()
//    object HandleBack : SpaceAddRoomActions()
}
