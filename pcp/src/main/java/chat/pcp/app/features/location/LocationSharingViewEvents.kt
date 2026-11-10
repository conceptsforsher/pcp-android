/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location

import chat.pcp.app.core.platform.PcpViewEvents

sealed class LocationSharingViewEvents : PcpViewEvents {
    object Close : LocationSharingViewEvents()
    object LocationNotAvailableError : LocationSharingViewEvents()
    data class ZoomToUserLocation(val userLocation: LocationData) : LocationSharingViewEvents()
    data class StartLiveLocationService(val sessionId: String, val roomId: String, val durationMillis: Long) : LocationSharingViewEvents()
    object ChooseLiveLocationDuration : LocationSharingViewEvents()
    object ShowLabsFlagPromotion : LocationSharingViewEvents()
    object LiveLocationSharingNotEnoughPermission : LocationSharingViewEvents()
}
