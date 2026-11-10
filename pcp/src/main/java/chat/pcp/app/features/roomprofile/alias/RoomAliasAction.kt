/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.alias

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.model.RoomDirectoryVisibility

sealed class RoomAliasAction : PcpViewModelAction {
    // Canonical
    object ToggleManualPublishForm : RoomAliasAction()
    data class SetNewAlias(val alias: String) : RoomAliasAction()
    object ManualPublishAlias : RoomAliasAction()
    data class PublishAlias(val alias: String) : RoomAliasAction()
    data class UnpublishAlias(val alias: String) : RoomAliasAction()
    data class SetCanonicalAlias(val canonicalAlias: String?) : RoomAliasAction()

    // Room directory
    data class SetRoomDirectoryVisibility(val roomDirectoryVisibility: RoomDirectoryVisibility) : RoomAliasAction()

    // Local
    data class RemoveLocalAlias(val alias: String) : RoomAliasAction()
    object ToggleAddLocalAliasForm : RoomAliasAction()
    data class SetNewLocalAliasLocalPart(val aliasLocalPart: String) : RoomAliasAction()
    object AddLocalAlias : RoomAliasAction()

    // Retry to fetch data in error
    object Retry : RoomAliasAction()
}
