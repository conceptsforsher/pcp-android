/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.debug.analytics

import chat.pcp.app.core.platform.PcpViewModelAction

sealed interface DebugAnalyticsViewActions : PcpViewModelAction {
    object ResetAnalyticsOptInDisplayed : DebugAnalyticsViewActions
}
