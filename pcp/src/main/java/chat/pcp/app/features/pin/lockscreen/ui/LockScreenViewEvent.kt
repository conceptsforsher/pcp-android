/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.pin.lockscreen.ui

import chat.pcp.app.core.platform.PcpViewEvents

sealed class LockScreenViewEvent : PcpViewEvents {
    data class ClearPinCode(val confirmationFailed: Boolean) : LockScreenViewEvent()
    object CodeCreationComplete : LockScreenViewEvent()
    data class AuthSuccessful(val method: AuthMethod) : LockScreenViewEvent()
    data class AuthFailure(val method: AuthMethod) : LockScreenViewEvent()
    data class AuthError(val method: AuthMethod, val throwable: Throwable) : LockScreenViewEvent()
    object ShowBiometricKeyInvalidatedMessage : LockScreenViewEvent()
    object ShowBiometricPromptAutomatically : LockScreenViewEvent()
}
