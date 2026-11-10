/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.people

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.model.RoomMemberSummary

sealed class SpacePeopleViewAction : PcpViewModelAction {
    data class ChatWith(val member: RoomMemberSummary) : SpacePeopleViewAction()
    object InviteToSpace : SpacePeopleViewAction()
}
