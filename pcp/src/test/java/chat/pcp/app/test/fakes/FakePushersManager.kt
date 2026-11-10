/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import chat.pcp.app.core.pushers.PushersManager
import io.mockk.coJustRun
import io.mockk.coVerify
import io.mockk.every
import io.mockk.mockk
import org.matrix.android.sdk.api.session.pushers.Pusher

class FakePushersManager {

    val instance = mockk<PushersManager>()

    fun givenGetPusherForCurrentSessionReturns(pusher: Pusher?) {
        every { instance.getPusherForCurrentSession() } returns pusher
    }

    fun givenUnregisterPusher(pushKey: String) {
        coJustRun { instance.unregisterPusher(pushKey) }
    }

    fun verifyUnregisterPusher(pushKey: String) {
        coVerify { instance.unregisterPusher(pushKey) }
    }
}
