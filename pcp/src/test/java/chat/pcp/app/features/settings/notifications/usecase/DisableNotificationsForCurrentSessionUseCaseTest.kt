/*
 * Copyright 2026, 2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.notifications.usecase

import chat.pcp.app.core.pushers.UnregisterUnifiedPushUseCase
import chat.pcp.app.test.fakes.FakePushersManager
import io.mockk.coJustRun
import io.mockk.coVerify
import io.mockk.mockk
import kotlinx.coroutines.test.runTest
import org.junit.Test

class DisableNotificationsForCurrentSessionUseCaseTest {

    private val fakePushersManager = FakePushersManager()
    private val fakeToggleNotificationsForCurrentSessionUseCase = mockk<ToggleNotificationsForCurrentSessionUseCase>()
    private val fakeUnregisterUnifiedPushUseCase = mockk<UnregisterUnifiedPushUseCase>()

    private val disableNotificationsForCurrentSessionUseCase = DisableNotificationsForCurrentSessionUseCase(
            pushersManager = fakePushersManager.instance,
            toggleNotificationsForCurrentSessionUseCase = fakeToggleNotificationsForCurrentSessionUseCase,
            unregisterUnifiedPushUseCase = fakeUnregisterUnifiedPushUseCase,
    )

    @Test
    fun `when execute then disable notifications and unregister the pusher`() = runTest {
        // Given
        coJustRun { fakeToggleNotificationsForCurrentSessionUseCase.execute(any()) }
        coJustRun { fakeUnregisterUnifiedPushUseCase.execute(any()) }

        // When
        disableNotificationsForCurrentSessionUseCase.execute()

        // Then
        coVerify {
            fakeToggleNotificationsForCurrentSessionUseCase.execute(false)
            fakeUnregisterUnifiedPushUseCase.execute(fakePushersManager.instance)
        }
    }
}
