/*
 * Copyright 2019-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.signout.soft

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.auth.data.Credentials

sealed class SoftLogoutAction : PcpViewModelAction {
    // In case of failure to get the login flow
    object RetryLoginFlow : SoftLogoutAction()

    // For password entering management
    data class PasswordChanged(val password: String) : SoftLogoutAction()
    data class SignInAgain(val password: String) : SoftLogoutAction()

    // For signing again with SSO
    data class WebLoginSuccess(val credentials: Credentials) : SoftLogoutAction()

    // To clear the current session
    object ClearData : SoftLogoutAction()
}
