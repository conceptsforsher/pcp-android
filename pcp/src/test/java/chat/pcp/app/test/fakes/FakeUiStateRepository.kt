/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import chat.pcp.app.features.ui.UiStateRepository
import io.mockk.justRun
import io.mockk.mockk
import io.mockk.verify
import org.matrix.android.sdk.api.session.Session

class FakeUiStateRepository : UiStateRepository by mockk() {

    init {
        justRun { storeSelectedSpace(any(), any()) }
    }

    fun verifyStoreSelectedSpace(roomId: String, session: Session, inverse: Boolean = false) {
        verify(inverse = inverse) { storeSelectedSpace(roomId, session.sessionId) }
    }
}
