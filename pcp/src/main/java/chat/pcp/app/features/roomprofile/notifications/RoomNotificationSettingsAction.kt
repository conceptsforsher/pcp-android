/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.notifications

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.notification.RoomNotificationState

sealed class RoomNotificationSettingsAction : PcpViewModelAction {
    data class SelectNotificationState(val notificationState: RoomNotificationState) : RoomNotificationSettingsAction()
}
