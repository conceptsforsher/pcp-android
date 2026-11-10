/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location.preview

import android.graphics.drawable.Drawable
import com.airbnb.mvrx.MavericksState
import chat.pcp.app.features.location.LocationData
import chat.pcp.app.features.location.LocationSharingArgs

data class LocationPreviewViewState(
        val pinLocationData: LocationData? = null,
        val roomId: String? = null,
        val pinUserId: String? = null,
        val pinDrawable: Drawable? = null,
        val loadingMapHasFailed: Boolean = false,
        val isLoadingUserLocation: Boolean = false,
        val lastKnownUserLocation: LocationData? = null,
) : MavericksState {

    constructor(args: LocationSharingArgs) : this(
            pinLocationData = args.initialLocationData,
            roomId = args.roomId,
            pinUserId = args.locationOwnerId,
    )
}
