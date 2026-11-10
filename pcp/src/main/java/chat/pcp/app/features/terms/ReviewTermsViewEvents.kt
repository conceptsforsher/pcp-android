/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.terms

import chat.pcp.app.core.platform.PcpViewEvents

sealed class ReviewTermsViewEvents : PcpViewEvents {
    data class Loading(val message: CharSequence? = null) : ReviewTermsViewEvents()
    data class Failure(val throwable: Throwable, val finish: Boolean) : ReviewTermsViewEvents()
    object Success : ReviewTermsViewEvents()
}
