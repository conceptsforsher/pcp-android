/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import chat.pcp.app.SpaceStateHandler
import io.mockk.mockk
import io.mockk.verify

class FakeSpaceStateHandler : SpaceStateHandler by mockk(relaxUnitFun = true) {

    fun verifySetCurrentSpace(spaceId: String) {
        verify { setCurrentSpace(spaceId) }
    }
}
