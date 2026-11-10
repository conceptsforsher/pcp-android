/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import chat.pcp.app.features.settings.BackgroundSyncMode
import chat.pcp.app.features.settings.PcpBasePreferences
import io.mockk.every
import io.mockk.justRun
import io.mockk.mockk
import io.mockk.verify

class FakePcpPreferences {

    val instance = mockk<PcpPreferences>(relaxUnitFun = true)

    fun givenUseCompleteNotificationFormat(value: Boolean) {
        every { instance.useCompleteNotificationFormat() } returns value
    }

    fun givenSpaceBackstack(value: List<String?>) {
        every { instance.getSpaceBackstack() } returns value
    }

    fun verifySetSpaceBackstack(value: List<String?>, inverse: Boolean = false) {
        verify(inverse = inverse) { instance.setSpaceBackstack(value) }
    }

    fun givenIsClientInfoRecordingEnabled(isEnabled: Boolean) {
        every { instance.isClientInfoRecordingEnabled() } returns isEnabled
    }

    fun givenTextFormatting(isEnabled: Boolean) =
            every { instance.isTextFormattingEnabled() } returns isEnabled

    fun givenSetNotificationEnabledForDevice() {
        justRun { instance.setNotificationEnabledForDevice(any()) }
    }

    fun verifySetNotificationEnabledForDevice(enabled: Boolean, inverse: Boolean = false) {
        verify(inverse = inverse) { instance.setNotificationEnabledForDevice(enabled) }
    }

    fun givenSessionManagerShowIpAddress(showIpAddress: Boolean) {
        every { instance.showIpAddressInSessionManagerScreens() } returns showIpAddress
    }

    fun givenUnverifiedSessionsAlertLastShownMillis(lastShownMillis: Long) {
        every { instance.getUnverifiedSessionsAlertLastShownMillis(any()) } returns lastShownMillis
    }

    fun givenSetFdroidSyncBackgroundMode(mode: BackgroundSyncMode) {
        justRun { instance.setFdroidSyncBackgroundMode(mode) }
    }

    fun verifySetFdroidSyncBackgroundMode(mode: BackgroundSyncMode) {
        verify { instance.setFdroidSyncBackgroundMode(mode) }
    }

    fun givenAreNotificationsEnabledForDevice(notificationsEnabled: Boolean) {
        every { instance.areNotificationEnabledForDevice() } returns notificationsEnabled
    }

    fun givenIsBackgroundSyncEnabled(isEnabled: Boolean) {
        every { instance.isBackgroundSyncEnabled() } returns isEnabled
    }

    fun givenShowIpAddressInSessionManagerScreens(show: Boolean) {
        every { instance.showIpAddressInSessionManagerScreens() } returns show
    }

    fun verifySetIpAddressVisibilityInDeviceManagerScreens(isVisible: Boolean) {
        verify { instance.setIpAddressVisibilityInDeviceManagerScreens(isVisible) }
    }

    fun givenIsVoiceBroadcastEnabled(isEnabled: Boolean) {
        every { instance.isVoiceBroadcastEnabled() } returns isEnabled
    }
}
