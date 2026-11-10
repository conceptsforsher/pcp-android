/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.permissions

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.powerlevels.UserPowerLevel

sealed class RoomPermissionsAction : PcpViewModelAction {
    object ToggleShowAllPermissions : RoomPermissionsAction()

    data class UpdatePermission(val editablePermission: EditablePermission, val powerLevel: UserPowerLevel.Value) : RoomPermissionsAction()
}
