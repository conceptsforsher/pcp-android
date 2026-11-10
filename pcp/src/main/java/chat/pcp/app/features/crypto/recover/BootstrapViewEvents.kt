/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.crypto.recover

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse

sealed class BootstrapViewEvents : PcpViewEvents {
    data class Dismiss(val success: Boolean) : BootstrapViewEvents()
    data class ModalError(val error: String) : BootstrapViewEvents()
    object RecoveryKeySaved : BootstrapViewEvents()
    data class SkipBootstrap(val genKeyOption: Boolean = true) : BootstrapViewEvents()
    data class RequestReAuth(val flowResponse: RegistrationFlowResponse, val lastErrorCode: String?) : BootstrapViewEvents()
}
