/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.rename

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class RenameSessionAction : PcpViewModelAction {
    object InitWithLastEditedName : RenameSessionAction()
    object SaveModifications : RenameSessionAction()
    data class EditLocally(val editedName: String) : RenameSessionAction()
}
