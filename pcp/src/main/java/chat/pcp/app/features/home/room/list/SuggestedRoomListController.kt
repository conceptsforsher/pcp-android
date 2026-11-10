/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

class SuggestedRoomListController(
        private val roomSummaryItemFactory: RoomSummaryItemFactory
) : CollapsableTypedEpoxyController<SuggestedRoomInfo>() {

    var listener: RoomListListener? = null

    override fun buildModels(data: SuggestedRoomInfo?) {
        data?.rooms?.forEach { info ->
            add(roomSummaryItemFactory.createSuggestion(info, data.joinEcho, listener))
        }
    }
}
