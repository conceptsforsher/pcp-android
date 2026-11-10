/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.permissions

import chat.pcp.app.core.platform.PcpViewEvents

/**
 * Transient events for room settings screen.
 */
sealed class RoomPermissionsViewEvents : PcpViewEvents {
    data class Failure(val throwable: Throwable) : RoomPermissionsViewEvents()
    object Success : RoomPermissionsViewEvents()
}
