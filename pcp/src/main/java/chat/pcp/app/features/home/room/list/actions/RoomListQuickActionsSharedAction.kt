/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.actions

import androidx.annotation.DrawableRes
import androidx.annotation.StringRes
import chat.pcp.app.R
import chat.pcp.app.core.platform.PcpSharedAction
import chat.pcp.lib.strings.CommonStrings

sealed class RoomListQuickActionsSharedAction(
        @StringRes val titleRes: Int,
        @DrawableRes val iconResId: Int?,
        val destructive: Boolean = false
) : PcpSharedAction {

    data class NotificationsAllNoisy(val roomId: String) : RoomListQuickActionsSharedAction(
            CommonStrings.room_list_quick_actions_notifications_all_noisy,
            R.drawable.ic_room_actions_notifications_all_noisy
    )

    data class NotificationsAll(val roomId: String) : RoomListQuickActionsSharedAction(
            CommonStrings.room_list_quick_actions_notifications_all,
            R.drawable.ic_room_actions_notifications_all
    )

    data class NotificationsMentionsOnly(val roomId: String) : RoomListQuickActionsSharedAction(
            CommonStrings.room_list_quick_actions_notifications_mentions,
            R.drawable.ic_room_actions_notifications_mentions
    )

    data class NotificationsMute(val roomId: String) : RoomListQuickActionsSharedAction(
            CommonStrings.room_list_quick_actions_notifications_mute,
            R.drawable.ic_room_actions_notifications_mutes
    )

    data class Settings(val roomId: String) : RoomListQuickActionsSharedAction(
            CommonStrings.room_list_quick_actions_settings,
            R.drawable.ic_room_actions_settings
    )

    data class LowPriority(val roomId: String) : RoomListQuickActionsSharedAction(
            CommonStrings.room_list_quick_actions_low_priority_add,
            R.drawable.ic_low_priority_24
    )

    data class Favorite(val roomId: String) : RoomListQuickActionsSharedAction(
            CommonStrings.room_list_quick_actions_favorite_add,
            R.drawable.ic_star_24dp
    )

    data class Leave(val roomId: String, val showIcon: Boolean = true) : RoomListQuickActionsSharedAction(
            CommonStrings.room_list_quick_actions_leave,
            if (showIcon) R.drawable.ic_room_actions_leave else null,
            true
    )
}
