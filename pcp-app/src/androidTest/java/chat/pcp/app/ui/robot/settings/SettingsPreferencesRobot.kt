/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.ui.robot.settings

import androidx.test.espresso.Espresso.pressBack
import androidx.test.espresso.matcher.ViewMatchers.withId
import androidx.test.espresso.matcher.ViewMatchers.withText
import com.adevinta.android.barista.interaction.BaristaClickInteractions.clickOn
import com.adevinta.android.barista.interaction.BaristaDialogInteractions.clickDialogNegativeButton
import chat.pcp.app.R
import chat.pcp.app.espresso.tools.waitUntilViewVisible
import chat.pcp.lib.strings.CommonStrings

class SettingsPreferencesRobot {

    fun crawl() {
        clickOn(CommonStrings.settings_interface_language)
        waitUntilViewVisible(withText("Dansk (Danmark)"))
        pressBack()
        clickOn(CommonStrings.settings_theme)
        clickDialogNegativeButton()
        clickOn(CommonStrings.font_size)
        waitUntilViewVisible(withId(R.id.fons_scale_recycler))
        pressBack()
    }
}
