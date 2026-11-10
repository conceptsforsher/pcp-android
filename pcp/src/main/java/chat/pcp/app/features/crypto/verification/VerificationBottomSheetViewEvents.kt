/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.crypto.verification

import chat.pcp.app.core.platform.PcpViewEvents

/**
 * Transient events for the verification bottom sheet.
 */
sealed class VerificationBottomSheetViewEvents : PcpViewEvents {
    object Dismiss : VerificationBottomSheetViewEvents()
    object DismissAndOpenDeviceSettings : VerificationBottomSheetViewEvents()
    object AccessSecretStore : VerificationBottomSheetViewEvents()
    object ResetAll : VerificationBottomSheetViewEvents()
    object GoToSettings : VerificationBottomSheetViewEvents()
    data class ConfirmCancel(val otherUserId: String, val deviceId: String?) : VerificationBottomSheetViewEvents()
    data class ModalError(val errorMessage: CharSequence) : VerificationBottomSheetViewEvents()
    data class RequestNotFound(val transactionId: String) : VerificationBottomSheetViewEvents()
}
