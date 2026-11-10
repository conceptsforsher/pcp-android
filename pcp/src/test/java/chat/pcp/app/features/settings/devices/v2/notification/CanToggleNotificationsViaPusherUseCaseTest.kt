/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.notification

import chat.pcp.app.test.fakes.FakeFlowLiveDataConversions
import chat.pcp.app.test.fakes.FakeSession
import chat.pcp.app.test.fakes.givenAsFlow
import chat.pcp.app.test.fixtures.aHomeServerCapabilities
import io.mockk.unmockkAll
import kotlinx.coroutines.flow.firstOrNull
import kotlinx.coroutines.test.runTest
import org.amshove.kluent.shouldBeEqualTo
import org.junit.After
import org.junit.Before
import org.junit.Test

private val A_HOMESERVER_CAPABILITIES = aHomeServerCapabilities(canRemotelyTogglePushNotificationsOfDevices = true)

class CanToggleNotificationsViaPusherUseCaseTest {

    private val fakeSession = FakeSession()
    private val fakeFlowLiveDataConversions = FakeFlowLiveDataConversions()

    private val canToggleNotificationsViaPusherUseCase =
            CanToggleNotificationsViaPusherUseCase()

    @Before
    fun setUp() {
        fakeFlowLiveDataConversions.setup()
    }

    @After
    fun tearDown() {
        unmockkAll()
    }

    @Test
    fun `given current session when execute then flow of the toggle capability is returned`() = runTest {
        // Given
        fakeSession
                .fakeHomeServerCapabilitiesService
                .givenCapabilitiesLiveReturns(A_HOMESERVER_CAPABILITIES)
                .givenAsFlow()

        // When
        val result = canToggleNotificationsViaPusherUseCase.execute(fakeSession).firstOrNull()

        // Then
        result shouldBeEqualTo A_HOMESERVER_CAPABILITIES.canRemotelyTogglePushNotificationsOfDevices
    }
}
