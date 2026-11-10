/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.widgets

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class WidgetAction : PcpViewModelAction {
    data class OnWebViewStartedToLoad(val url: String) : WidgetAction()
    data class OnWebViewLoadingError(val url: String, val isHttpError: Boolean, val errorCode: Int, val errorDescription: String) : WidgetAction()
    data class OnWebViewLoadingSuccess(val url: String) : WidgetAction()
    object LoadFormattedUrl : WidgetAction()
    object DeleteWidget : WidgetAction()
    object RevokeWidget : WidgetAction()
    object OnTermsReviewed : WidgetAction()
    object CloseWidget : WidgetAction()
}
