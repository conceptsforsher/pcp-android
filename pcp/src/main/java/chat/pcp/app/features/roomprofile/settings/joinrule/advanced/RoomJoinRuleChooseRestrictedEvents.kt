/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.settings.joinrule.advanced

import chat.pcp.app.core.platform.PcpViewEvents

sealed class RoomJoinRuleChooseRestrictedEvents : PcpViewEvents {
    object NavigateToChooseRestricted : RoomJoinRuleChooseRestrictedEvents()
    data class NavigateToUpgradeRoom(val roomId: String, val toVersion: String, val description: CharSequence) : RoomJoinRuleChooseRestrictedEvents()
}
