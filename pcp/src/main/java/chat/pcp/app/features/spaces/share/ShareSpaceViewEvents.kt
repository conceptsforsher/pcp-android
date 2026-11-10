/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.share

import chat.pcp.app.core.platform.PcpViewEvents

sealed class ShareSpaceViewEvents : PcpViewEvents {
    data class NavigateToInviteUser(val spaceId: String) : ShareSpaceViewEvents()
    data class ShowInviteByLink(val permalink: String, val spaceName: String) : ShareSpaceViewEvents()
}
