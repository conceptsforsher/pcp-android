/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.people

import chat.pcp.app.core.platform.PcpViewEvents

sealed class SpacePeopleViewEvents : PcpViewEvents {
    data class OpenRoom(val roomId: String) : SpacePeopleViewEvents()
    data class InviteToSpace(val spaceId: String) : SpacePeopleViewEvents()
}
