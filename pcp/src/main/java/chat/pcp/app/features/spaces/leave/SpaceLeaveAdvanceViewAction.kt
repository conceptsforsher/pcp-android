/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.leave

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class SpaceLeaveAdvanceViewAction : PcpViewModelAction {
    data class ToggleSelection(val roomId: String) : SpaceLeaveAdvanceViewAction()
    data class UpdateFilter(val filter: String) : SpaceLeaveAdvanceViewAction()
    data class SetFilteringEnabled(val isEnabled: Boolean) : SpaceLeaveAdvanceViewAction()
    object DoLeave : SpaceLeaveAdvanceViewAction()
    object ClearError : SpaceLeaveAdvanceViewAction()
    object SelectAll : SpaceLeaveAdvanceViewAction()
    object SelectNone : SpaceLeaveAdvanceViewAction()
}
