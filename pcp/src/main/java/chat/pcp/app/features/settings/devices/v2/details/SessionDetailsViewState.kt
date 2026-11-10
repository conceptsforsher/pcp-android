/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.details

import com.airbnb.mvrx.Async
import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.Uninitialized
import chat.pcp.app.features.settings.devices.v2.DeviceFullInfo

data class SessionDetailsViewState(
        val deviceId: String,
        val deviceFullInfo: Async<DeviceFullInfo> = Uninitialized,
) : MavericksState {
    constructor(args: SessionDetailsArgs) : this(
            deviceId = args.deviceId
    )
}
