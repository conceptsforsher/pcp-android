/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

import com.airbnb.epoxy.TypedEpoxyController
import chat.pcp.app.core.epoxy.helpFooterItem
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.resources.UserPreferencesProvider
import chat.pcp.app.features.home.RoomListDisplayMode
import chat.pcp.app.features.home.room.filtered.FilteredRoomFooterItem
import chat.pcp.app.features.home.room.filtered.filteredRoomFooterItem
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

class RoomListFooterController @Inject constructor(
        private val stringProvider: StringProvider,
        private val userPreferencesProvider: UserPreferencesProvider
) : TypedEpoxyController<RoomListViewState>() {

    var listener: FilteredRoomFooterItem.Listener? = null

    override fun buildModels(data: RoomListViewState?) {
        val host = this
        when (data?.displayMode) {
            RoomListDisplayMode.FILTERED -> {
                filteredRoomFooterItem {
                    id("filter_footer")
                    listener(host.listener)
                    currentFilter(data.roomFilter)
                    inSpace(data.asyncSelectedSpace.invoke() != null)
                }
            }
            else -> {
                if (userPreferencesProvider.shouldShowLongClickOnRoomHelp()) {
                    helpFooterItem {
                        id("long_click_help")
                        text(host.stringProvider.getString(CommonStrings.help_long_click_on_room_for_more_options))
                    }
                }
            }
        }
    }
}
