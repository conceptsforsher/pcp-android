/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import chat.pcp.app.config.Config
import chat.pcp.app.test.fakes.FakeClock
import chat.pcp.app.test.fakes.FakePcpFeatures
import chat.pcp.app.test.fakes.FakePcpPreferences
import org.amshove.kluent.shouldBe
import org.junit.Test

private val AN_EPOCH = Config.SHOW_UNVERIFIED_SESSIONS_ALERT_AFTER_MILLIS.toLong()
private const val A_DEVICE_ID = "A_DEVICE_ID"

class ShouldShowUnverifiedSessionsAlertUseCaseTest {

    private val fakePcpFeatures = FakePcpFeatures()
    private val fakePcpPreferences = FakePcpPreferences()
    private val fakeClock = FakeClock()

    private val shouldShowUnverifiedSessionsAlertUseCase = ShouldShowUnverifiedSessionsAlertUseCase(
            vectorFeatures = fakePcpFeatures,
            vectorPreferences = fakePcpPreferences.instance,
            clock = fakeClock,
    )

    @Test
    fun `given the feature is disabled then the use case returns false`() {
        fakePcpFeatures.givenUnverifiedSessionsAlertEnabled(false)
        fakePcpPreferences.givenUnverifiedSessionsAlertLastShownMillis(0L)

        shouldShowUnverifiedSessionsAlertUseCase.execute(A_DEVICE_ID) shouldBe false
    }

    @Test
    fun `given the feature in enabled and there is not a saved preference then the use case returns true`() {
        fakePcpFeatures.givenUnverifiedSessionsAlertEnabled(true)
        fakePcpPreferences.givenUnverifiedSessionsAlertLastShownMillis(0L)
        fakeClock.givenEpoch(AN_EPOCH + 1)

        shouldShowUnverifiedSessionsAlertUseCase.execute(A_DEVICE_ID) shouldBe true
    }

    @Test
    fun `given the feature in enabled and last shown is a long time ago then the use case returns true`() {
        fakePcpFeatures.givenUnverifiedSessionsAlertEnabled(true)
        fakePcpPreferences.givenUnverifiedSessionsAlertLastShownMillis(AN_EPOCH)
        fakeClock.givenEpoch(AN_EPOCH * 2 + 1)

        shouldShowUnverifiedSessionsAlertUseCase.execute(A_DEVICE_ID) shouldBe true
    }

    @Test
    fun `given the feature in enabled and last shown is not a long time ago then the use case returns false`() {
        fakePcpFeatures.givenUnverifiedSessionsAlertEnabled(true)
        fakePcpPreferences.givenUnverifiedSessionsAlertLastShownMillis(AN_EPOCH)
        fakeClock.givenEpoch(AN_EPOCH + 1)

        shouldShowUnverifiedSessionsAlertUseCase.execute(A_DEVICE_ID) shouldBe false
    }
}
