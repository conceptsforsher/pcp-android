/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.manage

import chat.pcp.app.core.platform.PcpViewEvents

sealed class SpaceAddRoomsViewEvents : PcpViewEvents {
    object WarnUnsavedChanged : SpaceAddRoomsViewEvents()
    object SavedDone : SpaceAddRoomsViewEvents()
    data class SaveFailed(val reason: Throwable) : SpaceAddRoomsViewEvents()
}
