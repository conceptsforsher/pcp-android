/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomdirectory

import chat.pcp.app.core.platform.PcpViewEvents

/**
 * Transient events for room directory screen.
 */
sealed class RoomDirectoryViewEvents : PcpViewEvents {
    data class Failure(val throwable: Throwable) : RoomDirectoryViewEvents()
}
