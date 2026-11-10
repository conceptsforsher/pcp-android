/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

import org.matrix.android.sdk.api.session.room.model.RoomSummary
import javax.inject.Inject

class AlphabeticalRoomComparator @Inject constructor() : Comparator<RoomSummary> {

    override fun compare(leftRoomSummary: RoomSummary?, rightRoomSummary: RoomSummary?): Int {
        return when {
            rightRoomSummary?.displayName == null -> -1
            leftRoomSummary?.displayName == null -> 1
            else -> leftRoomSummary.displayName.compareTo(rightRoomSummary.displayName)
        }
    }
}
