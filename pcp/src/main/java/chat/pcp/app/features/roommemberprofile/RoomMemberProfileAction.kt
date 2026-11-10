/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roommemberprofile

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.powerlevels.UserPowerLevel

sealed class RoomMemberProfileAction : PcpViewModelAction {
    object RetryFetchingInfo : RoomMemberProfileAction()
    object IgnoreUser : RoomMemberProfileAction()
    object ReportUser : RoomMemberProfileAction()
    data class BanOrUnbanUser(val reason: String?) : RoomMemberProfileAction()
    data class KickUser(val reason: String?) : RoomMemberProfileAction()
    object InviteUser : RoomMemberProfileAction()
    object VerifyUser : RoomMemberProfileAction()
    object ShareRoomMemberProfile : RoomMemberProfileAction()
    data class SetPowerLevel(val previousValue: UserPowerLevel, val newValue: UserPowerLevel.Value, val askForValidation: Boolean) : RoomMemberProfileAction()
    data class SetUserColorOverride(val newColorSpec: String) : RoomMemberProfileAction()
    data class OpenOrCreateDm(val userId: String) : RoomMemberProfileAction()
}
