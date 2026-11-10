/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.settings.troubleshoot

import chat.pcp.app.core.di.ActiveSessionHolder
import chat.pcp.app.core.error.ErrorFormatter
import chat.pcp.app.core.pushers.PushersManager
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.features.session.coroutineScope
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import org.matrix.android.sdk.api.session.pushers.PushGatewayFailure
import javax.inject.Inject

/**
 * Test Push by asking the Push Gateway to send a Push back.
 */
class TestPushFromPushGateway @Inject constructor(
        private val stringProvider: StringProvider,
        private val errorFormatter: ErrorFormatter,
        private val pushersManager: PushersManager,
        private val activeSessionHolder: ActiveSessionHolder,
) : TroubleshootTest(CommonStrings.settings_troubleshoot_test_push_loop_title) {

    private var action: Job? = null
    private var pushReceived: Boolean = false

    override fun perform(testParameters: TestParameters) {
        pushReceived = false
        action = activeSessionHolder.getActiveSession().coroutineScope.launch {
            val result = runCatching { pushersManager.testPush() }

            withContext(Dispatchers.Main) {
                status = result
                        .fold(
                                {
                                    if (pushReceived) {
                                        // Push already received (race condition)
                                        description = stringProvider.getString(CommonStrings.settings_troubleshoot_test_push_loop_success)
                                        TestStatus.SUCCESS
                                    } else {
                                        // Wait for the push to be received
                                        description = stringProvider.getString(CommonStrings.settings_troubleshoot_test_push_loop_waiting_for_push)
                                        TestStatus.RUNNING
                                    }
                                },
                                {
                                    description = if (it is PushGatewayFailure.PusherRejected) {
                                        stringProvider.getString(CommonStrings.settings_troubleshoot_test_push_loop_failed)
                                    } else {
                                        errorFormatter.toHumanReadable(it)
                                    }
                                    TestStatus.FAILED
                                }
                        )
            }
        }
    }

    override fun onPushReceived() {
        pushReceived = true
        description = stringProvider.getString(CommonStrings.settings_troubleshoot_test_push_loop_success)
        status = TestStatus.SUCCESS
    }

    override fun cancel() {
        action?.cancel()
    }
}
