/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.notification

import chat.pcp.app.features.settings.devices.v2.notification.NotificationsStatus
import chat.pcp.app.test.fakes.FakeGetNotificationsStatusUseCase
import chat.pcp.app.test.fakes.FakeSession
import chat.pcp.app.test.fakes.FakePcpPreferences
import kotlinx.coroutines.test.runTest
import org.junit.Test

private const val A_SESSION_ID = "session-id"

class UpdateEnableNotificationsSettingOnChangeUseCaseTest {

    private val fakeSession = FakeSession().also { it.givenSessionId(A_SESSION_ID) }
    private val fakePcpPreferences = FakePcpPreferences()
    private val fakeGetNotificationsStatusUseCase = FakeGetNotificationsStatusUseCase()

    private val updateEnableNotificationsSettingOnChangeUseCase = UpdateEnableNotificationsSettingOnChangeUseCase(
            vectorPreferences = fakePcpPreferences.instance,
            getNotificationsStatusUseCase = fakeGetNotificationsStatusUseCase.instance,
    )

    @Test
    fun `given notifications are enabled when execute then setting is updated to true`() = runTest {
        // Given
        fakeGetNotificationsStatusUseCase.givenExecuteReturns(
                fakeSession,
                A_SESSION_ID,
                NotificationsStatus.ENABLED,
        )
        fakePcpPreferences.givenSetNotificationEnabledForDevice()

        // When
        updateEnableNotificationsSettingOnChangeUseCase.execute(fakeSession)

        // Then
        fakePcpPreferences.verifySetNotificationEnabledForDevice(true)
    }

    @Test
    fun `given notifications are disabled when execute then setting is updated to false`() = runTest {
        // Given
        fakeGetNotificationsStatusUseCase.givenExecuteReturns(
                fakeSession,
                A_SESSION_ID,
                NotificationsStatus.DISABLED,
        )
        fakePcpPreferences.givenSetNotificationEnabledForDevice()

        // When
        updateEnableNotificationsSettingOnChangeUseCase.execute(fakeSession)

        // Then
        fakePcpPreferences.verifySetNotificationEnabledForDevice(false)
    }

    @Test
    fun `given notifications toggle is not supported when execute then nothing is done`() = runTest {
        // Given
        fakeGetNotificationsStatusUseCase.givenExecuteReturns(
                fakeSession,
                A_SESSION_ID,
                NotificationsStatus.NOT_SUPPORTED,
        )
        fakePcpPreferences.givenSetNotificationEnabledForDevice()

        // When
        updateEnableNotificationsSettingOnChangeUseCase.execute(fakeSession)

        // Then
        fakePcpPreferences.verifySetNotificationEnabledForDevice(true, inverse = true)
        fakePcpPreferences.verifySetNotificationEnabledForDevice(false, inverse = true)
    }
}
