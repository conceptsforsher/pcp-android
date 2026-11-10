/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.notification

import chat.pcp.app.test.fakes.FakeSession
import chat.pcp.app.test.fixtures.aHomeServerCapabilities
import org.amshove.kluent.shouldBeEqualTo
import org.junit.Test

private val A_HOMESERVER_CAPABILITIES = aHomeServerCapabilities(canRemotelyTogglePushNotificationsOfDevices = true)

class CheckIfCanToggleNotificationsViaPusherUseCaseTest {

    private val fakeSession = FakeSession()

    private val checkIfCanToggleNotificationsViaPusherUseCase =
            CheckIfCanToggleNotificationsViaPusherUseCase()

    @Test
    fun `given current session when execute then toggle capability is returned`() {
        // Given
        fakeSession
                .fakeHomeServerCapabilitiesService
                .givenCapabilities(A_HOMESERVER_CAPABILITIES)

        // When
        val result = checkIfCanToggleNotificationsViaPusherUseCase.execute(fakeSession)

        // Then
        result shouldBeEqualTo A_HOMESERVER_CAPABILITIES.canRemotelyTogglePushNotificationsOfDevices
    }
}
