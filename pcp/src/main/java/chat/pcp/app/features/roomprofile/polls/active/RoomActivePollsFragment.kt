/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls.active

import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.features.roomprofile.polls.RoomPollsType
import chat.pcp.app.features.roomprofile.polls.list.ui.RoomPollsListFragment
import chat.pcp.lib.strings.CommonPlurals
import chat.pcp.lib.strings.CommonStrings

@AndroidEntryPoint
class RoomActivePollsFragment : RoomPollsListFragment() {

    override fun getEmptyListTitle(canLoadMore: Boolean, nbLoadedDays: Int): String {
        return if (canLoadMore) {
            stringProvider.getQuantityString(CommonPlurals.room_polls_active_no_item_for_loaded_period, nbLoadedDays, nbLoadedDays)
        } else {
            getString(CommonStrings.room_polls_active_no_item)
        }
    }

    override fun getRoomPollsType() = RoomPollsType.ACTIVE
}
