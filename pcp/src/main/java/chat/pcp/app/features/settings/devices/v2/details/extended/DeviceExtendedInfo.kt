/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.details.extended

import chat.pcp.app.features.settings.devices.v2.list.DeviceType

data class DeviceExtendedInfo(
        /**
         * One of MOBILE, WEB, DESKTOP or UNKNOWN.
         */
        val deviceType: DeviceType,
        /**
         * i.e. Google Pixel 6.
         */
        val deviceModel: String? = null,
        /**
         * i.e. Android 11.
         */
        val deviceOperatingSystem: String? = null,
        /**
         * i.e. Element Nightly.
         */
        val clientName: String? = null,
        /**
         * i.e. 1.5.0.
         */
        val clientVersion: String? = null,
)
