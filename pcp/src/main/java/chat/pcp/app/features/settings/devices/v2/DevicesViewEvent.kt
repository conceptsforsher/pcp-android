/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse
import org.matrix.android.sdk.api.session.crypto.model.CryptoDeviceInfo

sealed class DevicesViewEvent : PcpViewEvents {
    data class RequestReAuth(
            val registrationFlowResponse: RegistrationFlowResponse,
            val lastErrorCode: String?
    ) : DevicesViewEvent()

    data class ShowVerifyDevice(val userId: String, val transactionId: String?) : DevicesViewEvent()
    object SelfVerification : DevicesViewEvent()
    data class ShowManuallyVerify(val cryptoDeviceInfo: CryptoDeviceInfo) : DevicesViewEvent()
    object PromptResetSecrets : DevicesViewEvent()
    object SignoutSuccess : DevicesViewEvent()
    data class SignoutError(val error: Throwable) : DevicesViewEvent()
}
