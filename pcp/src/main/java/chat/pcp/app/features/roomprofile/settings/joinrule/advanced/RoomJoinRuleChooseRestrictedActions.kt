/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.settings.joinrule.advanced

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.model.RoomJoinRules
import org.matrix.android.sdk.api.util.MatrixItem

sealed class RoomJoinRuleChooseRestrictedActions : PcpViewModelAction {
    data class FilterWith(val filter: String) : RoomJoinRuleChooseRestrictedActions()
    data class ToggleSelection(val matrixItem: MatrixItem) : RoomJoinRuleChooseRestrictedActions()
    data class SelectJoinRules(val rules: RoomJoinRules) : RoomJoinRuleChooseRestrictedActions()
    object DoUpdateJoinRules : RoomJoinRuleChooseRestrictedActions()
    data class SwitchToRoomAfterMigration(val roomId: String) : RoomJoinRuleChooseRestrictedActions()
}
