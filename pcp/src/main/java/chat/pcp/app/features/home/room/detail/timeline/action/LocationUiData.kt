/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.action

import chat.pcp.app.features.home.room.detail.timeline.helper.LocationPinProvider

/**
 * Data used to display Location data in the message bottom sheet.
 */
data class LocationUiData(
        val locationUrl: String,
        val locationOwnerId: String?,
        val locationPinProvider: LocationPinProvider,
)
