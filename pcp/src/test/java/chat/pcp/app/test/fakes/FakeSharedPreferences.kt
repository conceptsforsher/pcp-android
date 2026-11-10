/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import android.content.SharedPreferences
import chat.pcp.app.features.settings.FontScaleValue
import io.mockk.every
import io.mockk.mockk

class FakeSharedPreferences : SharedPreferences by mockk() {

    fun givenFontScaleIsSetTo(fontScaleValue: FontScaleValue) {
        every { contains("APPLICATION_FONT_SCALE_KEY") } returns true
        every { getString("APPLICATION_FONT_SCALE_KEY", any()) } returns fontScaleValue.preferenceValue
    }

    fun givenUseSystemFontScaleIsSetTo(useSystemScale: Boolean) {
        every { contains("APPLICATION_USE_SYSTEM_FONT_SCALE_KEY") } returns true
        every { getBoolean("APPLICATION_USE_SYSTEM_FONT_SCALE_KEY", any()) } returns useSystemScale
    }
}
