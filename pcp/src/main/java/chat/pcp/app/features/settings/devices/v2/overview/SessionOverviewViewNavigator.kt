/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.overview

import android.content.Context
import androidx.fragment.app.FragmentActivity
import chat.pcp.app.features.settings.devices.v2.details.SessionDetailsActivity
import chat.pcp.app.features.settings.devices.v2.rename.RenameSessionActivity
import javax.inject.Inject

class SessionOverviewViewNavigator @Inject constructor() {

    fun goToSessionDetails(context: Context, deviceId: String) {
        context.startActivity(SessionDetailsActivity.newIntent(context, deviceId))
    }

    fun goToRenameSession(context: Context, deviceId: String) {
        context.startActivity(RenameSessionActivity.newIntent(context, deviceId))
    }

    fun goBack(fragmentActivity: FragmentActivity) {
        fragmentActivity.finish()
    }
}
