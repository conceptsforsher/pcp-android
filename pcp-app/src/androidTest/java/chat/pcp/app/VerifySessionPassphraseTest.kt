/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app

import android.view.View
import androidx.recyclerview.widget.RecyclerView
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.action.ViewActions.click
import androidx.test.espresso.action.ViewActions.typeText
import androidx.test.espresso.assertion.ViewAssertions.matches
import androidx.test.espresso.contrib.RecyclerViewActions.actionOnItem
import androidx.test.espresso.matcher.ViewMatchers.hasDescendant
import androidx.test.espresso.matcher.ViewMatchers.isDisplayed
import androidx.test.espresso.matcher.ViewMatchers.isRoot
import androidx.test.espresso.matcher.ViewMatchers.withId
import androidx.test.espresso.matcher.ViewMatchers.withText
import androidx.test.ext.junit.rules.ActivityScenarioRule
import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.filters.LargeTest
import androidx.test.platform.app.InstrumentationRegistry
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.utils.getMatrixInstance
import chat.pcp.app.espresso.tools.waitUntilActivityVisible
import chat.pcp.app.espresso.tools.waitUntilViewVisible
import chat.pcp.app.features.MainActivity
import chat.pcp.app.features.crypto.quads.SharedSecureStorageActivity
import chat.pcp.app.features.crypto.recover.BootstrapCrossSigningTask
import chat.pcp.app.features.crypto.recover.Params
import chat.pcp.app.features.crypto.recover.SetupMode
import chat.pcp.app.features.home.HomeActivity
import chat.pcp.app.ui.robot.AnalyticsRobot
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.runBlocking
import org.junit.Before
import org.junit.Ignore
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import org.matrix.android.sdk.api.auth.UIABaseAuth
import org.matrix.android.sdk.api.auth.UserInteractiveAuthInterceptor
import org.matrix.android.sdk.api.auth.UserPasswordAuth
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse
import org.matrix.android.sdk.api.session.Session
import kotlin.coroutines.Continuation
import kotlin.coroutines.resume
import kotlin.random.Random

@RunWith(AndroidJUnit4::class)
@LargeTest
@Ignore
class VerifySessionPassphraseTest : VerificationTestBase() {

    var existingSession: Session? = null
    val passphrase = "person woman camera tv"

    @get:Rule
    val activityRule = ActivityScenarioRule(MainActivity::class.java)

    @Before
    fun createSessionWithCrossSigningAnd4S() {
        val context = InstrumentationRegistry.getInstrumentation().targetContext
        val matrix = getMatrixInstance()
        val userName = "foobar_${Random.nextLong()}"
        existingSession = createAccountAndSync(matrix, userName, password, true)
        runBlockingTest {
            existingSession!!.cryptoService().crossSigningService()
                    .initializeCrossSigning(
                            object : UserInteractiveAuthInterceptor {
                                override fun performStage(flowResponse: RegistrationFlowResponse, errCode: String?, promise: Continuation<UIABaseAuth>) {
                                    promise.resume(
                                            UserPasswordAuth(
                                                    user = existingSession!!.myUserId,
                                                    password = "password",
                                                    session = flowResponse.session
                                            )
                                    )
                                }
                            }
                    )
        }

        val task = BootstrapCrossSigningTask(existingSession!!, StringProvider(context.resources))

        runBlocking {
            task.execute(
                    Params(
                            userInteractiveAuthInterceptor = object : UserInteractiveAuthInterceptor {
                                override fun performStage(flowResponse: RegistrationFlowResponse, errCode: String?, promise: Continuation<UIABaseAuth>) {
                                    promise.resume(
                                            UserPasswordAuth(
                                                    user = existingSession!!.myUserId,
                                                    password = password,
                                                    session = flowResponse.session
                                            )
                                    )
                                }
                            },
                            passphrase = passphrase,
                            setupMode = SetupMode.NORMAL
                    )
            )
        }
    }

    @Test
    fun checkVerifyWithPassphrase() {
        val userId: String = existingSession!!.myUserId

        uiTestBase.login(userId = userId, password = password, homeServerUrl = homeServerUrl)

        val analyticsRobot = AnalyticsRobot()
        analyticsRobot.optOut()

        waitUntilActivityVisible<HomeActivity> {
            waitUntilViewVisible(withId(R.id.roomListContainer))
        }

        val activity = EspressoHelper.getCurrentActivity()!!
        val uiSession = (activity as HomeActivity).activeSessionHolder.getActiveSession()
        withIdlingResource(initialSyncIdlingResource(uiSession)) {
            waitUntilViewVisible(withId(R.id.roomListContainer))
        }

        // THIS IS THE ONLY WAY I FOUND TO CLICK ON ALERTERS... :(
        // Cannot wait for view because of alerter animation? ...
        onView(isRoot())
                .perform(waitForView(withId(com.tapadoo.alerter.R.id.llAlertBackground)))

        Thread.sleep(1000)
        val popup = activity.findViewById<View>(com.tapadoo.alerter.R.id.llAlertBackground)
        activity.runOnUiThread {
            popup.performClick()
        }

        onView(isRoot())
                .perform(waitForView(withId(R.id.bottomSheetFragmentContainer)))

        onView(withText(CommonStrings.verification_verify_identity))
                .check(matches(isDisplayed()))

        // 4S is  setup so passphrase option should be visible
        onView(withId(R.id.bottomSheetVerificationRecyclerView))
                .check(matches((hasDescendant(withText(CommonStrings.verification_cannot_access_other_session)))))

        onView(withId(R.id.bottomSheetVerificationRecyclerView))
                .perform(
                        actionOnItem<RecyclerView.ViewHolder>(
                                hasDescendant(withText(CommonStrings.verification_cannot_access_other_session)),
                                click()
                        )
                )

        withIdlingResource(activityIdlingResource(SharedSecureStorageActivity::class.java)) {
            onView(withId(R.id.ssss__root)).check(matches(isDisplayed()))
        }

        onView(withId(R.id.ssss_passphrase_enter_edittext))
                .perform(typeText(passphrase))

        onView(withId(R.id.ssss_passphrase_submit))
                .perform(click())

        System.out.println("*** passphrase 1")

        withIdlingResource(activityIdlingResource(HomeActivity::class.java)) {
            System.out.println("*** passphrase 1.1")
            onView(withId(R.id.bottomSheetVerificationRecyclerView))
                    .perform(waitForView(hasDescendant(withText(CommonStrings.verification_conclusion_ok_notice))))
        }

        // click on done
        onView(withId(R.id.bottomSheetVerificationRecyclerView))
                .perform(
                        actionOnItem<RecyclerView.ViewHolder>(
                                hasDescendant(withText(CommonStrings.done)),
                                click()
                        )
                )

        System.out.println("*** passphrase 2")
        // check that all secrets are known?
        assert(uiSession.cryptoService().crossSigningService().canCrossSign())
        assert(uiSession.cryptoService().crossSigningService().allPrivateKeysKnown())

        Thread.sleep(10_000)
    }
}
