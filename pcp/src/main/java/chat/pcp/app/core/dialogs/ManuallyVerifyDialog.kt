/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.dialogs

import android.app.Activity
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import chat.pcp.app.R
import chat.pcp.app.databinding.DialogDeviceVerifyBinding
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.extensions.getFingerprintHumanReadable
import org.matrix.android.sdk.api.session.crypto.model.CryptoDeviceInfo

object ManuallyVerifyDialog {

    fun show(activity: Activity, cryptoDeviceInfo: CryptoDeviceInfo, onVerified: (() -> Unit)) {
        val dialogLayout = activity.layoutInflater.inflate(R.layout.dialog_device_verify, null)
        val views = DialogDeviceVerifyBinding.bind(dialogLayout)
        val builder = MaterialAlertDialogBuilder(activity)
                .setTitle(CommonStrings.cross_signing_verify_by_text)
                .setView(dialogLayout)
                .setPositiveButton(CommonStrings.encryption_information_verify) { _, _ ->
                    onVerified()
                }
                .setNegativeButton(CommonStrings.action_cancel, null)

        views.encryptedDeviceInfoDeviceName.text = cryptoDeviceInfo.displayName()
        views.encryptedDeviceInfoDeviceId.text = cryptoDeviceInfo.deviceId
        views.encryptedDeviceInfoDeviceKey.text = cryptoDeviceInfo.getFingerprintHumanReadable()

        builder.show()
    }
}
