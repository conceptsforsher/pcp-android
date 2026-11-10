/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2

import chat.pcp.app.test.fakes.FakePcpPreferences
import org.junit.Test

class ToggleIpAddressVisibilityUseCaseTest {

    private val fakePcpPreferences = FakePcpPreferences()

    private val toggleIpAddressVisibilityUseCase = ToggleIpAddressVisibilityUseCase(
            vectorPreferences = fakePcpPreferences.instance,
    )

    @Test
    fun `given ip addresses are currently visible then then visibility is set as false`() {
        // Given
        fakePcpPreferences.givenShowIpAddressInSessionManagerScreens(true)

        // When
        toggleIpAddressVisibilityUseCase.execute()

        // Then
        fakePcpPreferences.verifySetIpAddressVisibilityInDeviceManagerScreens(false)
    }

    @Test
    fun `given ip addresses are currently not visible then then visibility is set as true`() {
        // Given
        fakePcpPreferences.givenShowIpAddressInSessionManagerScreens(false)

        // When
        toggleIpAddressVisibilityUseCase.execute()

        // Then
        fakePcpPreferences.verifySetIpAddressVisibilityInDeviceManagerScreens(true)
    }
}
