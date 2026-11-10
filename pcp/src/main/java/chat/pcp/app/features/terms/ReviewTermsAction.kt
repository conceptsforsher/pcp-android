/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.terms

import chat.pcp.app.core.platform.PcpViewModelAction

sealed class ReviewTermsAction : PcpViewModelAction {
    data class LoadTerms(val preferredLanguageCode: String) : ReviewTermsAction()
    data class MarkTermAsAccepted(val url: String, val accepted: Boolean) : ReviewTermsAction()
    object Accept : ReviewTermsAction()
}
