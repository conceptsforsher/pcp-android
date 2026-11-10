/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomdirectory.createroom

import chat.pcp.app.core.platform.PcpViewEvents

/**
 * Transient events for room creation screen.
 */
sealed class CreateRoomViewEvents : PcpViewEvents {
    data class Failure(val throwable: Throwable) : CreateRoomViewEvents()
    object Quit : CreateRoomViewEvents()
}
