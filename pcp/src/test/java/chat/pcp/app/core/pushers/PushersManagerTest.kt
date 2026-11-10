/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.pushers

import chat.pcp.app.features.mdm.NoOpMdmService
import chat.pcp.app.test.fakes.FakeActiveSessionHolder
import chat.pcp.app.test.fakes.FakeAppNameProvider
import chat.pcp.app.test.fakes.FakeGetDeviceInfoUseCase
import chat.pcp.app.test.fakes.FakeLocaleProvider
import chat.pcp.app.test.fakes.FakePushersService
import chat.pcp.app.test.fakes.FakeSession
import chat.pcp.app.test.fakes.FakeStringProvider
import chat.pcp.app.test.fixtures.CredentialsFixture
import chat.pcp.app.test.fixtures.CryptoDeviceInfoFixture.aCryptoDeviceInfo
import chat.pcp.app.test.fixtures.PusherFixture
import chat.pcp.app.test.fixtures.SessionParamsFixture
import io.mockk.mockk
import kotlinx.coroutines.test.runTest
import org.amshove.kluent.shouldBeEqualTo
import org.junit.Test
import org.matrix.android.sdk.api.session.crypto.model.UnsignedDeviceInfo
import org.matrix.android.sdk.api.session.pushers.HttpPusher
import java.util.Locale
import kotlin.math.abs

class PushersManagerTest {

    private val pushersService = FakePushersService()
    private val session = FakeSession(fakePushersService = pushersService)
    private val activeSessionHolder = FakeActiveSessionHolder(session)
    private val stringProvider = FakeStringProvider()
    private val localeProvider = FakeLocaleProvider()
    private val appNameProvider = FakeAppNameProvider()
    private val getDeviceInfoUseCase = FakeGetDeviceInfoUseCase()

    private val pushersManager = PushersManager(
            mockk(),
            activeSessionHolder.instance,
            localeProvider,
            stringProvider.instance,
            appNameProvider,
            getDeviceInfoUseCase,
            NoOpMdmService(),
    )

    @Test
    fun `when enqueueRegisterPusher, then HttpPusher created and enqueued`() = runTest {
        val pushKey = "abc"
        val gateway = "123"
        val pusherAppId = "app-id"
        val appName = "element"
        val deviceDisplayName = "iPhone Lollipop"
        stringProvider.given(chat.pcp.app.config.R.string.pusher_app_id, pusherAppId)
        localeProvider.givenCurrent(Locale.UK)
        appNameProvider.givenAppName(appName)
        getDeviceInfoUseCase.givenDeviceInfo(aCryptoDeviceInfo(unsigned = UnsignedDeviceInfo(deviceDisplayName)))
        val expectedHttpPusher = HttpPusher(
                pushkey = pushKey,
                appId = pusherAppId,
                profileTag = DEFAULT_PUSHER_FILE_TAG + "_" + abs(session.myUserId.hashCode()),
                lang = Locale.UK.language,
                appDisplayName = appName,
                deviceDisplayName = deviceDisplayName,
                url = gateway,
                enabled = true,
                deviceId = session.sessionParams.deviceId,
                append = false,
                withEventIdOnly = true,
        )

        pushersManager.enqueueRegisterPusher(pushKey, gateway)

        val httpPusher = pushersService.verifyEnqueueAddHttpPusher()
        httpPusher shouldBeEqualTo expectedHttpPusher
    }

    @Test
    fun `when getPusherForCurrentSession, then return pusher`() {
        val deviceId = "device_id"
        val sessionParams = SessionParamsFixture.aSessionParams(
                credentials = CredentialsFixture.aCredentials(deviceId = deviceId)
        )
        session.givenSessionParams(sessionParams)
        val expectedPusher = PusherFixture.aPusher(deviceId = deviceId)
        pushersService.givenGetPushers(listOf(expectedPusher))

        val pusher = pushersManager.getPusherForCurrentSession()

        pusher shouldBeEqualTo expectedPusher
    }
}
