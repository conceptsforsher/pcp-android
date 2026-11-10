/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roommemberprofile

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.session.room.powerlevels.UserPowerLevel

/**
 * Transient events for RoomMemberProfile.
 */
sealed class RoomMemberProfileViewEvents : PcpViewEvents {
    data class Loading(val message: CharSequence? = null) : RoomMemberProfileViewEvents()
    data class Failure(val throwable: Throwable) : RoomMemberProfileViewEvents()

    object OnIgnoreActionSuccess : RoomMemberProfileViewEvents()
    object OnReportActionSuccess : RoomMemberProfileViewEvents()
    object OnSetPowerLevelSuccess : RoomMemberProfileViewEvents()
    object OnInviteActionSuccess : RoomMemberProfileViewEvents()
    object OnKickActionSuccess : RoomMemberProfileViewEvents()
    object OnBanActionSuccess : RoomMemberProfileViewEvents()
    data class ShowPowerLevelValidation(val currentValue: UserPowerLevel, val newValue: UserPowerLevel.Value) : RoomMemberProfileViewEvents()
    data class ShowPowerLevelDemoteWarning(val currentValue: UserPowerLevel, val newValue: UserPowerLevel.Value) : RoomMemberProfileViewEvents()

    data class StartVerification(
            val userId: String,
            val canCrossSign: Boolean
    ) : RoomMemberProfileViewEvents()

    data class ShareRoomMemberProfile(val permalink: String) : RoomMemberProfileViewEvents()
    data class OpenRoom(val roomId: String) : RoomMemberProfileViewEvents()
    object GoBack : RoomMemberProfileViewEvents()
}
