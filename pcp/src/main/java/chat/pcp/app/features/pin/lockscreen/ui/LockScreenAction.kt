/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.pin.lockscreen.ui

import androidx.fragment.app.FragmentActivity
import chat.pcp.app.core.platform.PcpViewModelAction

sealed class LockScreenAction : PcpViewModelAction {
    data class PinCodeEntered(val value: String) : LockScreenAction()
    data class ShowBiometricPrompt(val callingActivity: FragmentActivity) : LockScreenAction()
    object OnUIReady : LockScreenAction()
}
