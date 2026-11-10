/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import chat.pcp.app.core.pushers.FcmHelper
import chat.pcp.app.core.pushers.PushersManager
import io.mockk.justRun
import io.mockk.mockk
import io.mockk.verify

class FakeFcmHelper {

    val instance = mockk<FcmHelper>()

    fun givenEnsureFcmTokenIsRetrieved(pushersManager: PushersManager) {
        justRun { instance.ensureFcmTokenIsRetrieved(pushersManager, any()) }
    }

    fun verifyEnsureFcmTokenIsRetrieved(
            pushersManager: PushersManager,
            registerPusher: Boolean,
            inverse: Boolean = false,
    ) {
        verify(inverse = inverse) { instance.ensureFcmTokenIsRetrieved(pushersManager, registerPusher) }
    }
}
