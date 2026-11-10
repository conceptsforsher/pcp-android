/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.account.deactivation

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class DeactivateAccountAction : PcpViewModelAction {
    data class DeactivateAccount(val eraseAllData: Boolean) : DeactivateAccountAction()

    object SsoAuthDone : DeactivateAccountAction()
    data class PasswordAuthDone(val password: String) : DeactivateAccountAction()
    object ReAuthCancelled : DeactivateAccountAction()
}
