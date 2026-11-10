/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.ui.robot

import androidx.test.espresso.Espresso.pressBack
import androidx.test.espresso.matcher.ViewMatchers.withId
import com.adevinta.android.barista.interaction.BaristaClickInteractions.clickOn
import chat.pcp.app.R
import chat.pcp.app.espresso.tools.waitUntilViewVisible
import chat.pcp.app.features.DefaultPcpFeatures
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.ui.robot.settings.labs.LabFeaturesPreferences
import chat.pcp.lib.strings.CommonStrings

class NewRoomRobot(
        var createdRoom: Boolean = false,
        private val labsPreferences: LabFeaturesPreferences
) {
    private val features: PcpFeatures = DefaultPcpFeatures()

    fun createNewRoom(block: CreateNewRoomRobot.() -> Unit) {
        clickOn(CommonStrings.create_new_room)
        waitUntilViewVisible(withId(R.id.createRoomForm))
        val createNewRoomRobot = CreateNewRoomRobot()
        block(createNewRoomRobot)
        createdRoom = createNewRoomRobot.createdRoom
        if (!createNewRoomRobot.createdRoom) {
            pressBack()
        }
    }
}
