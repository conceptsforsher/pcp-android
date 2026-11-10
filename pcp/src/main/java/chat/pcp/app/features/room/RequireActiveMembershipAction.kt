/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.room

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class RequireActiveMembershipAction : PcpViewModelAction {
    data class ChangeRoom(val roomId: String) : RequireActiveMembershipAction()
}
