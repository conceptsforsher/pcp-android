/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location.preview

import chat.pcp.app.core.platform.PcpViewEvents
import chat.pcp.app.features.location.LocationData

sealed class LocationPreviewViewEvents : PcpViewEvents {
    data class ZoomToUserLocation(val userLocation: LocationData) : LocationPreviewViewEvents()
    object UserLocationNotAvailableError : LocationPreviewViewEvents()
}
