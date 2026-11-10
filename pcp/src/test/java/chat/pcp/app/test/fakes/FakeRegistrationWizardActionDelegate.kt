/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import chat.pcp.app.features.onboarding.RegisterAction
import chat.pcp.app.features.onboarding.RegistrationResult
import chat.pcp.app.features.onboarding.RegistrationWizardActionDelegate
import io.mockk.coEvery
import io.mockk.mockk

class FakeRegistrationWizardActionDelegate {

    val instance = mockk<RegistrationWizardActionDelegate>()

    fun givenResultsFor(result: List<Pair<RegisterAction, RegistrationResult>>) {
        coEvery { instance.executeAction(any()) } answers { call ->
            val actionArg = call.invocation.args[0] as RegisterAction
            result.first { it.first == actionArg }.second
        }
    }

    fun givenThrowsFor(action: RegisterAction, cause: Throwable) {
        coEvery { instance.executeAction(action) } throws cause
    }
}
