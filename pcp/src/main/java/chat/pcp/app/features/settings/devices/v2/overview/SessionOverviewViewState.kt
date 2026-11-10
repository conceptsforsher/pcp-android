/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.overview

import com.airbnb.mvrx.Async
import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.Uninitialized
import chat.pcp.app.features.settings.devices.v2.DeviceFullInfo
import chat.pcp.app.features.settings.devices.v2.notification.NotificationsStatus

data class SessionOverviewViewState(
        val deviceId: String,
        val isCurrentSessionTrusted: Boolean = false,
        val deviceInfo: Async<DeviceFullInfo> = Uninitialized,
        val isLoading: Boolean = false,
        val notificationsStatus: NotificationsStatus = NotificationsStatus.NOT_SUPPORTED,
        val isShowingIpAddress: Boolean = false,
        val externalDeleteDeviceUrl: String? = null,
) : MavericksState {
    constructor(args: SessionOverviewArgs) : this(
            deviceId = args.deviceId
    )
}
