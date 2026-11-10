/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import chat.pcp.app.features.onboarding.FtueUseCase
import chat.pcp.app.features.session.VectorSessionStore
import io.mockk.coEvery
import io.mockk.mockk

class FakePcpStore {
    val instance = mockk<VectorSessionStore>()

    fun givenUseCase(useCase: FtueUseCase?) {
        coEvery {
            instance.readUseCase()
        } coAnswers {
            useCase
        }
    }
}
