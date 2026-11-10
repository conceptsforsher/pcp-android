/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.signout

import android.content.Context
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

class BuildConfirmSignoutDialogUseCase @Inject constructor() {

    fun execute(context: Context, onConfirm: () -> Unit) =
            MaterialAlertDialogBuilder(context)
                    .setTitle(CommonStrings.action_sign_out)
                    .setMessage(CommonStrings.action_sign_out_confirmation_simple)
                    .setPositiveButton(CommonStrings.action_sign_out) { _, _ ->
                        onConfirm()
                    }
                    .setNegativeButton(CommonStrings.action_cancel, null)
                    .create()
}
