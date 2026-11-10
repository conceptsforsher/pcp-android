/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.ui.robot.space

import androidx.recyclerview.widget.RecyclerView
import androidx.test.espresso.Espresso
import androidx.test.espresso.action.ViewActions
import androidx.test.espresso.contrib.RecyclerViewActions
import androidx.test.espresso.matcher.ViewMatchers
import chat.pcp.app.R
import chat.pcp.app.espresso.tools.waitUntilActivityVisible
import chat.pcp.app.espresso.tools.waitUntilViewVisible
import chat.pcp.app.features.roomprofile.settings.joinrule.RoomJoinRuleActivity
import chat.pcp.lib.strings.CommonStrings

class SpaceSettingsRobot {
    fun crawl() {
        Espresso.onView(ViewMatchers.withId(R.id.roomSettingsRecyclerView))
                .perform(
                        RecyclerViewActions.actionOnItem<RecyclerView.ViewHolder>(
                                ViewMatchers.hasDescendant(ViewMatchers.withText(CommonStrings.room_settings_space_access_title)),
                                ViewActions.click()
                        )
                )

        waitUntilActivityVisible<RoomJoinRuleActivity> {
            waitUntilViewVisible(ViewMatchers.withId(R.id.genericRecyclerView))
        }

        Espresso.pressBack()

        Espresso.onView(ViewMatchers.withId(R.id.roomSettingsRecyclerView))
                .perform(
                        RecyclerViewActions.actionOnItem<RecyclerView.ViewHolder>(
                                ViewMatchers.hasDescendant(ViewMatchers.withText(CommonStrings.space_settings_manage_rooms)),
                                ViewActions.click()
                        )
                )

        waitUntilViewVisible(ViewMatchers.withId(R.id.roomList))
        Espresso.pressBack()

        Espresso.onView(ViewMatchers.withId(R.id.roomSettingsRecyclerView))
                .perform(
                        RecyclerViewActions.actionOnItem<RecyclerView.ViewHolder>(
                                ViewMatchers.hasDescendant(ViewMatchers.withText(CommonStrings.space_settings_permissions_title)),
                                ViewActions.click()
                        )
                )

        waitUntilViewVisible(ViewMatchers.withId(R.id.roomSettingsRecyclerView))
        Espresso.pressBack()
        Espresso.pressBack()
    }
}
