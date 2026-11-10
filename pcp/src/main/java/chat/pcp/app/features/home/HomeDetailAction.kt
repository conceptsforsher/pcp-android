/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class HomeDetailAction : PcpViewModelAction {
    data class SwitchTab(val tab: HomeTab) : HomeDetailAction()
    object MarkAllRoomsRead : HomeDetailAction()
    data class StartCallWithPhoneNumber(val phoneNumber: String) : HomeDetailAction()
}
