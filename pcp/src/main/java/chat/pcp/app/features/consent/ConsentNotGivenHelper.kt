/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.consent

import android.app.Activity
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import chat.pcp.app.core.dialogs.DialogLocker
import chat.pcp.app.core.platform.Restorable
import chat.pcp.app.features.webview.PcpWebViewActivity
import chat.pcp.app.features.webview.WebViewMode
import chat.pcp.lib.strings.CommonStrings

class ConsentNotGivenHelper(
        private val activity: Activity,
        private val dialogLocker: DialogLocker
) :
        Restorable by dialogLocker {

    /* ==========================================================================================
     * Public methods
     * ========================================================================================== */

    /**
     * Display the consent dialog, if not already displayed.
     */
    fun displayDialog(consentUri: String, homeServerHost: String) {
        dialogLocker.displayDialog {
            MaterialAlertDialogBuilder(activity)
                    .setTitle(CommonStrings.settings_app_term_conditions)
                    .setMessage(activity.getString(CommonStrings.dialog_user_consent_content, homeServerHost))
                    .setPositiveButton(CommonStrings.dialog_user_consent_submit) { _, _ ->
                        openWebViewActivity(consentUri)
                    }
        }
    }

    /* ==========================================================================================
     * Private
     * ========================================================================================== */

    private fun openWebViewActivity(consentUri: String) {
        val intent = PcpWebViewActivity.getIntent(activity, consentUri, activity.getString(CommonStrings.settings_app_term_conditions), WebViewMode.CONSENT)
        activity.startActivity(intent)
    }
}
