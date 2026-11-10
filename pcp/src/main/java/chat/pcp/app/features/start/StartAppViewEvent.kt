/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.start

import chat.pcp.app.core.platform.PcpViewEvents

sealed interface StartAppViewEvent : PcpViewEvents {
    /**
     * Will be sent if the process is taking more than 1 second.
     */
    object StartForegroundService : StartAppViewEvent

    /**
     * Will be sent when the current Session has been set.
     */
    object AppStarted : StartAppViewEvent
}
