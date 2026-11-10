/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.crypto.model.CryptoDeviceInfo

sealed class DevicesAction : PcpViewModelAction {
    object Refresh : DevicesAction()
    data class Delete(val deviceId: String) : DevicesAction()

    //    data class Password(val password: String) : DevicesAction()
    data class Rename(val deviceId: String, val newName: String) : DevicesAction()

    data class PromptRename(val deviceId: String) : DevicesAction()
    data class VerifyMyDevice(val deviceId: String) : DevicesAction()
    data class VerifyMyDeviceManually(val deviceId: String) : DevicesAction()
    object CompleteSecurity : DevicesAction()
    object ResetSecurity : DevicesAction()
    data class MarkAsManuallyVerified(val cryptoDeviceInfo: CryptoDeviceInfo) : DevicesAction()

    object SsoAuthDone : DevicesAction()
    data class PasswordAuthDone(val password: String) : DevicesAction()
    object ReAuthCancelled : DevicesAction()
}
