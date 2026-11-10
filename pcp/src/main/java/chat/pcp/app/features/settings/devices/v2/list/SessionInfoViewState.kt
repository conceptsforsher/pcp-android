/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.list

import chat.pcp.app.features.settings.devices.v2.DeviceFullInfo

data class SessionInfoViewState(
        val isCurrentSession: Boolean,
        val deviceFullInfo: DeviceFullInfo,
        val isVerifyButtonVisible: Boolean = true,
        val isDetailsButtonVisible: Boolean = true,
        val isLearnMoreLinkVisible: Boolean = false,
        val isLastActivityVisible: Boolean = false,
        val isShowingIpAddress: Boolean = false,
)
