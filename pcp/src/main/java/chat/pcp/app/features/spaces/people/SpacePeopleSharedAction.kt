/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.people

import chat.pcp.app.core.platform.PcpSharedAction

sealed class SpacePeopleSharedAction : PcpSharedAction {
    object Dismiss : SpacePeopleSharedAction()
    object ShowModalLoading : SpacePeopleSharedAction()
    object HideModalLoading : SpacePeopleSharedAction()
    data class NavigateToRoom(val roomId: String) : SpacePeopleSharedAction()
    data class NavigateToInvite(val spaceId: String) : SpacePeopleSharedAction()
}
