/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.details

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class SessionDetailsAction : PcpViewModelAction {
    data class CopyToClipboard(val content: String) : SessionDetailsAction()
}
