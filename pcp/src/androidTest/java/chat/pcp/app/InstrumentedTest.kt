/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app

import android.content.Context
import androidx.test.core.app.ApplicationProvider
import chat.pcp.app.test.shared.createTimberTestRule
import org.junit.Rule

interface InstrumentedTest {

    @Rule
    fun timberTestRule() = createTimberTestRule()

    fun context(): Context {
        return ApplicationProvider.getApplicationContext()
    }
}
