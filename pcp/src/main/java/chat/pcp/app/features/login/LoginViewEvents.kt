/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.login

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.auth.registration.FlowResult

/**
 * Transient events for Login.
 */
sealed class LoginViewEvents : PcpViewEvents {
    data class Loading(val message: CharSequence? = null) : LoginViewEvents()
    data class Failure(val throwable: Throwable) : LoginViewEvents()

    data class RegistrationFlowResult(val flowResult: FlowResult, val isRegistrationStarted: Boolean) : LoginViewEvents()
    object OutdatedHomeserver : LoginViewEvents()

    // Navigation event

    object OpenServerSelection : LoginViewEvents()
    data class OnServerSelectionDone(val serverType: ServerType) : LoginViewEvents()
    object OnLoginFlowRetrieved : LoginViewEvents()
    data class OnSignModeSelected(val signMode: SignMode) : LoginViewEvents()
    object OnForgetPasswordClicked : LoginViewEvents()
    object OnResetPasswordSendThreePidDone : LoginViewEvents()
    object OnResetPasswordMailConfirmationSuccess : LoginViewEvents()
    object OnResetPasswordMailConfirmationSuccessDone : LoginViewEvents()

    data class OnSendEmailSuccess(val email: String) : LoginViewEvents()
    data class OnSendMsisdnSuccess(val msisdn: String) : LoginViewEvents()

    data class OnWebLoginError(val errorCode: Int, val description: String, val failingUrl: String) : LoginViewEvents()
}
