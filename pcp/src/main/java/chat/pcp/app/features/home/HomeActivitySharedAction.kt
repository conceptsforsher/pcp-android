/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import chat.pcp.app.core.platform.PcpSharedAction

/**
 * Supported navigation actions for [HomeActivity].
 */
sealed class HomeActivitySharedAction : PcpSharedAction {
    object OpenDrawer : HomeActivitySharedAction()
    object CloseDrawer : HomeActivitySharedAction()
    object OnCloseSpace : HomeActivitySharedAction()
    object AddSpace : HomeActivitySharedAction()
    data class OpenSpacePreview(val spaceId: String) : HomeActivitySharedAction()
    data class OpenSpaceInvite(val spaceId: String) : HomeActivitySharedAction()
    data class ShowSpaceSettings(val spaceId: String) : HomeActivitySharedAction()
    object SendSpaceFeedBack : HomeActivitySharedAction()
}
