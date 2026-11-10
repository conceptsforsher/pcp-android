/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import chat.pcp.app.core.resources.AppNameProvider
import io.mockk.every
import io.mockk.mockk

class FakeAppNameProvider : AppNameProvider by mockk() {

    fun givenAppName(appName: String) {
        every { getAppName() } returns appName
    }
}
