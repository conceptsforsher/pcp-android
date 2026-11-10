/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces

import chat.pcp.app.core.platform.PcpViewEvents

/**
 * Transient events for group list screen.
 */
sealed class SpaceListViewEvents : PcpViewEvents {
    data class OpenSpaceSummary(val id: String) : SpaceListViewEvents()
    data class OpenSpaceInvite(val id: String) : SpaceListViewEvents()
    object AddSpace : SpaceListViewEvents()
    object CloseDrawer : SpaceListViewEvents()
}
