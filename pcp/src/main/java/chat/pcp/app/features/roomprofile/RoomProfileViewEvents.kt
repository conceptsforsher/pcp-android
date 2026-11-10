/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile

import androidx.core.content.pm.ShortcutInfoCompat
import chat.pcp.app.core.platform.PcpViewEvents

/**
 * Transient events for RoomProfile.
 */
sealed class RoomProfileViewEvents : PcpViewEvents {
    data class Loading(val message: CharSequence? = null) : RoomProfileViewEvents()
    object DismissLoading : RoomProfileViewEvents()
    data class Failure(val throwable: Throwable) : RoomProfileViewEvents()
    data class Success(val message: CharSequence) : RoomProfileViewEvents()

    data class ShareRoomProfile(val permalink: String) : RoomProfileViewEvents()
    data class OnShortcutReady(val shortcutInfo: ShortcutInfoCompat) : RoomProfileViewEvents()
}
