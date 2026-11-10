/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomdirectory

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.model.roomdirectory.PublicRoom

sealed class RoomDirectoryAction : PcpViewModelAction {
    data class SetRoomDirectoryData(val roomDirectoryData: RoomDirectoryData) : RoomDirectoryAction()
    data class FilterWith(val filter: String) : RoomDirectoryAction()
    object LoadMore : RoomDirectoryAction()
    data class JoinRoom(val publicRoom: PublicRoom) : RoomDirectoryAction()
}
