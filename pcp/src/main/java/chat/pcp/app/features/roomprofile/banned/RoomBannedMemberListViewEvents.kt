/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.banned

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.session.room.model.RoomMemberSummary

sealed class RoomBannedMemberListViewEvents : PcpViewEvents {
    data class ShowBannedInfo(val bannedByUserId: String, val banReason: String, val roomMemberSummary: RoomMemberSummary) : RoomBannedMemberListViewEvents()
    data class ToastError(val info: String) : RoomBannedMemberListViewEvents()
}
