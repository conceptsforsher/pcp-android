/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.devtools

import chat.pcp.app.core.platform.PcpViewEvents

sealed class DevToolsViewEvents : PcpViewEvents {
    object Dismiss : DevToolsViewEvents()

    //    object ShowStateList : DevToolsViewEvents()
    data class ShowAlertMessage(val message: String) : DevToolsViewEvents()
    data class ShowSnackMessage(val message: String) : DevToolsViewEvents()
}
