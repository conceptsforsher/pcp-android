/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

import androidx.core.util.Predicate
import org.matrix.android.sdk.api.session.room.model.RoomSummary
import javax.inject.Inject

class RoomListNameFilter @Inject constructor() : Predicate<RoomSummary> {

    var filter: String = ""

    override fun test(roomSummary: RoomSummary): Boolean {
        if (filter.isEmpty()) {
            // No filter
            return true
        }

        return roomSummary.displayName.contains(filter, ignoreCase = true)
    }
}
