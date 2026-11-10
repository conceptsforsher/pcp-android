/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home.release

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class ReleaseNotesAction : PcpViewModelAction {
    data class NextPressed(
            val isLastItemSelected: Boolean = false
    ) : ReleaseNotesAction()
    data class PageSelected(
            val selectedPageIndex: Int  = 0
    ) : ReleaseNotesAction()
}
