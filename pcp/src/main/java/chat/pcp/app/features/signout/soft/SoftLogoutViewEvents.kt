/*
 * Copyright 2019-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.signout.soft

import chat.pcp.app.core.platform.PcpViewEvents

/**
 * Transient events for SoftLogout.
 */
sealed class SoftLogoutViewEvents : PcpViewEvents {
    data class Failure(val throwable: Throwable) : SoftLogoutViewEvents()

    data class ErrorNotSameUser(val currentUserId: String, val newUserId: String) : SoftLogoutViewEvents()
    object ClearData : SoftLogoutViewEvents()
}
