/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.auth

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class ReAuthActions : PcpViewModelAction {
    object StartSSOFallback : ReAuthActions()
    object FallBackPageLoaded : ReAuthActions()
    object FallBackPageClosed : ReAuthActions()
    data class ReAuthWithPass(val password: String) : ReAuthActions()
}
