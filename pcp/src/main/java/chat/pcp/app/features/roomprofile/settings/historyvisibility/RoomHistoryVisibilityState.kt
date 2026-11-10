/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.settings.historyvisibility

import chat.pcp.app.core.ui.bottomsheet.BottomSheetGenericState
import org.matrix.android.sdk.api.session.room.model.RoomHistoryVisibility

data class RoomHistoryVisibilityState(
        val currentRoomHistoryVisibility: RoomHistoryVisibility = RoomHistoryVisibility.SHARED
) : BottomSheetGenericState() {

    constructor(args: RoomHistoryVisibilityBottomSheetArgs) : this(currentRoomHistoryVisibility = args.currentRoomHistoryVisibility)
}
