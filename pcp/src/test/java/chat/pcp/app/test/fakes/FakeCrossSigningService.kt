/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import io.mockk.coEvery
import io.mockk.mockk
import org.matrix.android.sdk.api.session.crypto.crosssigning.CrossSigningService

class FakeCrossSigningService : CrossSigningService by mockk() {

    fun givenIsCrossSigningInitializedReturns(isInitialized: Boolean) {
        coEvery { isCrossSigningInitialized() } returns isInitialized
    }

    fun givenIsCrossSigningVerifiedReturns(isVerified: Boolean) {
        coEvery { isCrossSigningVerified() } returns isVerified
    }
}
