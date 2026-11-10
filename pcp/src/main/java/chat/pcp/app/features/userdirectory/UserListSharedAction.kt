/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.userdirectory

import chat.pcp.app.core.platform.PcpSharedAction

sealed class UserListSharedAction : PcpSharedAction {
    object Close : UserListSharedAction()
    object GoBack : UserListSharedAction()
    data class OnMenuItemSubmitClick(val selections: Set<PendingSelection>) : UserListSharedAction()
    object OpenPhoneBook : UserListSharedAction()
    object AddByQrCode : UserListSharedAction()
}
