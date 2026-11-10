/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.notification.RoomNotificationState

sealed class RoomProfileAction : PcpViewModelAction {
    object EnableEncryption : RoomProfileAction()
    object LeaveRoom : RoomProfileAction()
    data class ChangeRoomNotificationState(val notificationState: RoomNotificationState) : RoomProfileAction()
    object ShareRoomProfile : RoomProfileAction()
    object CreateShortcut : RoomProfileAction()
    object RestoreEncryptionState : RoomProfileAction()
    data class SetEncryptToVerifiedDeviceOnly(val enabled: Boolean) : RoomProfileAction()
    data class ReportRoom(val reason: String) : RoomProfileAction()
}
