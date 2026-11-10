/*
 * Copyright 2026, 2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.lib.core.utils.test.fakes

import chat.pcp.lib.core.utils.timer.Clock
import io.mockk.every
import io.mockk.mockk

class FakeClock : Clock by mockk() {
    fun givenEpoch(epoch: Long) {
        every { epochMillis() } returns epoch
    }
}
