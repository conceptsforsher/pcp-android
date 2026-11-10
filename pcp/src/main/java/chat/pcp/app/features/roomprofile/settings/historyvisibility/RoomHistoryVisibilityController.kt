/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.settings.historyvisibility

import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.ui.bottomsheet.BottomSheetGenericController
import chat.pcp.app.features.home.room.detail.timeline.format.RoomHistoryVisibilityFormatter
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.session.room.model.RoomHistoryVisibility
import javax.inject.Inject

class RoomHistoryVisibilityController @Inject constructor(
        private val historyVisibilityFormatter: RoomHistoryVisibilityFormatter,
        private val stringProvider: StringProvider
) : BottomSheetGenericController<RoomHistoryVisibilityState, RoomHistoryVisibilityRadioAction>() {

    override fun getTitle() = stringProvider.getString(CommonStrings.room_settings_room_read_history_rules_pref_dialog_title)

    override fun getSubTitle() = stringProvider.getString(CommonStrings.room_settings_room_read_history_dialog_subtitle)

    override fun getActions(state: RoomHistoryVisibilityState): List<RoomHistoryVisibilityRadioAction> {
        return listOf(
                RoomHistoryVisibility.WORLD_READABLE,
                RoomHistoryVisibility.SHARED,
                RoomHistoryVisibility.INVITED,
                RoomHistoryVisibility.JOINED
        )
                .map { roomHistoryVisibility ->
                    RoomHistoryVisibilityRadioAction(
                            roomHistoryVisibility = roomHistoryVisibility,
                            title = historyVisibilityFormatter.getSetting(roomHistoryVisibility),
                            isSelected = roomHistoryVisibility == state.currentRoomHistoryVisibility
                    )
                }
    }
}
