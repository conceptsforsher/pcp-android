/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.crosssigning

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class CrossSigningSettingsAction : PcpViewModelAction {
    object InitializeCrossSigning : CrossSigningSettingsAction()
    object SsoAuthDone : CrossSigningSettingsAction()
    data class PasswordAuthDone(val password: String) : CrossSigningSettingsAction()
    object ReAuthCancelled : CrossSigningSettingsAction()
}
