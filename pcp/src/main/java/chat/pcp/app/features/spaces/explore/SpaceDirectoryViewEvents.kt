/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.explore

import chat.pcp.app.core.platform.PcpViewEvents

sealed class SpaceDirectoryViewEvents : PcpViewEvents {
    object Dismiss : SpaceDirectoryViewEvents()
    data class NavigateToRoom(val roomId: String) : SpaceDirectoryViewEvents()
    data class NavigateToMxToBottomSheet(val link: String) : SpaceDirectoryViewEvents()
    data class NavigateToCreateNewRoom(val currentSpaceId: String) : SpaceDirectoryViewEvents()
}
