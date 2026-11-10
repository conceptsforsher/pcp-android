/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class LocationSharingAction : PcpViewModelAction {
    object CurrentUserLocationSharing : LocationSharingAction()
    data class PinnedLocationSharing(val locationData: LocationData?) : LocationSharingAction()
    data class LocationTargetChange(val locationData: LocationData) : LocationSharingAction()
    object ZoomToUserLocation : LocationSharingAction()
    object LiveLocationSharingRequested : LocationSharingAction()
    data class StartLiveLocationSharing(val durationMillis: Long) : LocationSharingAction()
    object ShowMapLoadingError : LocationSharingAction()
}
