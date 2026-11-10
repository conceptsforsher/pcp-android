/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.create

import android.net.Uri
import chat.pcp.app.core.platform.PcpViewModelAction

sealed class CreateSpaceAction : PcpViewModelAction {
    data class SetRoomType(val type: SpaceType) : CreateSpaceAction()
    data class NameChanged(val name: String) : CreateSpaceAction()
    data class TopicChanged(val topic: String) : CreateSpaceAction()
    data class SpaceAliasChanged(val aliasLocalPart: String) : CreateSpaceAction()
    data class SetAvatar(val uri: Uri?) : CreateSpaceAction()
    object OnBackPressed : CreateSpaceAction()
    object NextFromDetails : CreateSpaceAction()
    object NextFromDefaultRooms : CreateSpaceAction()
    object NextFromAdd3pid : CreateSpaceAction()
    data class DefaultRoomNameChanged(val index: Int, val name: String) : CreateSpaceAction()
    data class DefaultInvite3pidChanged(val index: Int, val email: String) : CreateSpaceAction()
    data class SetSpaceTopology(val topology: SpaceTopology) : CreateSpaceAction()
}
