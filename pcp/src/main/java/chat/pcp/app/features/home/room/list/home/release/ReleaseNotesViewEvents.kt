/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home.release

import chat.pcp.app.core.platform.PcpViewEvents

sealed class ReleaseNotesViewEvents : PcpViewEvents {
    object Close : ReleaseNotesViewEvents()
    data class SelectPage(val index: Int) : ReleaseNotesViewEvents()
}
