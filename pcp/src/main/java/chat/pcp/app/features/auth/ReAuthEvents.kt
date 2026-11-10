/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.auth

import chat.pcp.app.core.platform.PcpViewEvents

sealed class ReAuthEvents : PcpViewEvents {
    data class OpenSsoURl(val url: String) : ReAuthEvents()
    object Dismiss : ReAuthEvents()
    data class PasswordFinishSuccess(val passwordSafeForIntent: String) : ReAuthEvents()
}
