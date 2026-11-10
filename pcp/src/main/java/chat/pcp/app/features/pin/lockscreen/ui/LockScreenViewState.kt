/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.pin.lockscreen.ui

import com.airbnb.mvrx.MavericksState
import chat.pcp.app.features.pin.lockscreen.configuration.LockScreenConfiguration

data class LockScreenViewState(
        val lockScreenConfiguration: LockScreenConfiguration,
        val canUseBiometricAuth: Boolean,
        val showBiometricPromptAutomatically: Boolean,
        val pinCodeState: PinCodeState,
        val isBiometricKeyInvalidated: Boolean,
) : MavericksState {
    constructor(lockScreenConfiguration: LockScreenConfiguration) : this(
            lockScreenConfiguration, false, false, PinCodeState.Idle, false,
    )
}

sealed class PinCodeState {
    object Idle : PinCodeState()
    data class FirstCodeEntered(val pinCode: String) : PinCodeState()
}
