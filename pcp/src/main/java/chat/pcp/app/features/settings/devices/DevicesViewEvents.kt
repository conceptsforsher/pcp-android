/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.crypto.model.CryptoDeviceInfo
import org.matrix.android.sdk.api.session.crypto.model.DeviceInfo

/**
 * Transient events for Ignored users screen.
 */
sealed class DevicesViewEvents : PcpViewEvents {
    data class Loading(val message: CharSequence? = null) : DevicesViewEvents()

    //    object HideLoading : DevicesViewEvents()
    data class Failure(val throwable: Throwable) : DevicesViewEvents()

//    object RequestPassword : DevicesViewEvents()

    data class RequestReAuth(val registrationFlowResponse: RegistrationFlowResponse, val lastErrorCode: String?) : DevicesViewEvents()

    data class PromptRenameDevice(val deviceInfo: DeviceInfo) : DevicesViewEvents()

    data class ShowVerifyDevice(
            val userId: String,
            val transactionId: String?
    ) : DevicesViewEvents()

    data class SelfVerification(
            val session: Session
    ) : DevicesViewEvents()

    data class ShowManuallyVerify(val cryptoDeviceInfo: CryptoDeviceInfo) : DevicesViewEvents()

    object PromptResetSecrets : DevicesViewEvents()

    data class OpenBrowser(val url: String) : DevicesViewEvents()
}
