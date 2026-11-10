/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.device

import chat.pcp.app.test.fakes.FakeActiveSessionHolder
import chat.pcp.app.test.fakes.FakeCryptoService
import chat.pcp.app.test.fakes.FakeSession
import kotlinx.coroutines.test.runTest
import org.amshove.kluent.shouldBeEqualTo
import org.junit.Test

class DefaultGetDeviceInfoUseCaseTest {

    private val cryptoService = FakeCryptoService()
    private val session = FakeSession(fakeCryptoService = cryptoService)
    private val activeSessionHolder = FakeActiveSessionHolder(session)

    private val getDeviceInfoUseCase = DefaultGetDeviceInfoUseCase(activeSessionHolder.instance)

    @Test
    fun `when execute, then get crypto device info`() = runTest {
        val result = getDeviceInfoUseCase.execute()

        result shouldBeEqualTo cryptoService.cryptoDeviceInfo
    }
}
