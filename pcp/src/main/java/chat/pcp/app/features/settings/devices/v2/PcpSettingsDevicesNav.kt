/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2

import android.content.Context
import chat.pcp.app.features.settings.devices.v2.filter.DeviceManagerFilterType
import chat.pcp.app.features.settings.devices.v2.othersessions.OtherSessionsActivity
import chat.pcp.app.features.settings.devices.v2.overview.SessionOverviewActivity
import chat.pcp.app.features.settings.devices.v2.rename.RenameSessionActivity
import javax.inject.Inject

class PcpSettingsDevicesNav @Inject constructor() {

    fun navigateToSessionOverview(context: Context, deviceId: String) {
        context.startActivity(SessionOverviewActivity.newIntent(context, deviceId))
    }

    fun navigateToOtherSessions(
            context: Context,
            defaultFilter: DeviceManagerFilterType,
            excludeCurrentDevice: Boolean,
    ) {
        context.startActivity(
                OtherSessionsActivity.newIntent(context, defaultFilter, excludeCurrentDevice)
        )
    }

    fun navigateToRenameSession(context: Context, deviceId: String) {
        context.startActivity(RenameSessionActivity.newIntent(context, deviceId))
    }
}
