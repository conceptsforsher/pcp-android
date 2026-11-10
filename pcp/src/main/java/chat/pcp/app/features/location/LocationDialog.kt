/*
 * Copyright 2023, 2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location

import androidx.fragment.app.Fragment
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import chat.pcp.lib.strings.CommonStrings

fun Fragment.showUserLocationNotAvailableErrorDialog(onConfirmListener: () -> Unit) {
    MaterialAlertDialogBuilder(requireActivity())
            .setTitle(CommonStrings.location_not_available_dialog_title)
            .setMessage(CommonStrings.location_not_available_dialog_content)
            .setPositiveButton(CommonStrings.ok) { _, _ ->
                onConfirmListener()
            }
            .setCancelable(false)
            .show()
}
