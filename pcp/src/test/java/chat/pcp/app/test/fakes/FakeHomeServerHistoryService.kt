/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import io.mockk.justRun
import io.mockk.mockk
import org.matrix.android.sdk.api.auth.HomeServerHistoryService

class FakeHomeServerHistoryService : HomeServerHistoryService by mockk() {
    override fun getKnownServersUrls() = emptyList<String>()
    fun expectUrlToBeAdded(url: String) {
        justRun { addHomeServerToHistory(url) }
    }
}
