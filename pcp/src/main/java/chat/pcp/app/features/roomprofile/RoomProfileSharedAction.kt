/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile

import chat.pcp.app.core.platform.PcpSharedAction

/**
 * Supported navigation actions for [RoomProfileActivity].
 */
sealed class RoomProfileSharedAction : PcpSharedAction {
    object OpenRoomSettings : RoomProfileSharedAction()
    object OpenRoomAliasesSettings : RoomProfileSharedAction()
    object OpenRoomPermissionsSettings : RoomProfileSharedAction()
    object OpenRoomPolls : RoomProfileSharedAction()
    object OpenRoomUploads : RoomProfileSharedAction()
    object OpenRoomMembers : RoomProfileSharedAction()
    object OpenBannedRoomMembers : RoomProfileSharedAction()
    object OpenRoomNotificationSettings : RoomProfileSharedAction()
}
