/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

import chat.pcp.app.features.home.RoomListDisplayMode
import chat.pcp.app.features.settings.FontScalePreferences
import org.matrix.android.sdk.api.session.room.model.RoomSummary

class RoomSummaryListController(
        private val roomSummaryItemFactory: RoomSummaryItemFactory,
        private val displayMode: RoomListDisplayMode,
        fontScalePreferences: FontScalePreferences
) : CollapsableTypedEpoxyController<List<RoomSummary>>() {

    var listener: RoomListListener? = null
    private val shouldUseSingleLine: Boolean

    init {
        val fontScale = fontScalePreferences.getResolvedFontScaleValue()
        shouldUseSingleLine = fontScale.scale > FontScalePreferences.SCALE_LARGE
    }

    override fun buildModels(data: List<RoomSummary>?) {
        data?.forEach {
            add(roomSummaryItemFactory.create(it, emptyMap(), emptySet(), displayMode, listener, shouldUseSingleLine))
        }
    }
}
