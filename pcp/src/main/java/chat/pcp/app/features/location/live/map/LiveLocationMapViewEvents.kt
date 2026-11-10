/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location.live.map

import chat.pcp.app.core.platform.PcpViewEvents
import chat.pcp.app.features.location.LocationData

sealed interface LiveLocationMapViewEvents : PcpViewEvents {
    data class LiveLocationError(val error: Throwable) : LiveLocationMapViewEvents
    data class ZoomToUserLocation(val userLocation: LocationData) : LiveLocationMapViewEvents
    object UserLocationNotAvailableError : LiveLocationMapViewEvents
}
