/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.actions

import chat.pcp.app.features.roomprofile.notifications.RoomNotificationSettingsViewState

data class RoomListQuickActionViewState(
        val roomListActionsArgs: RoomListActionsArgs,
        val notificationSettingsViewState: RoomNotificationSettingsViewState
)
