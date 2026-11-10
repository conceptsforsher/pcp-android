/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.ignored

import chat.pcp.app.core.platform.PcpViewEvents

/**
 * Transient events for Ignored users screen.
 */
sealed class IgnoredUsersViewEvents : PcpViewEvents {
    data class Loading(val message: CharSequence? = null) : IgnoredUsersViewEvents()
    data class Failure(val throwable: Throwable) : IgnoredUsersViewEvents()
    object Success : IgnoredUsersViewEvents()
}
