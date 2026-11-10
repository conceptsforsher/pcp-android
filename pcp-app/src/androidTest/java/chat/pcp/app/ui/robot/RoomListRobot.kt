/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.ui.robot

import androidx.recyclerview.widget.RecyclerView
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.Espresso.pressBack
import androidx.test.espresso.action.ViewActions
import androidx.test.espresso.contrib.RecyclerViewActions
import androidx.test.espresso.matcher.ViewMatchers.hasDescendant
import androidx.test.espresso.matcher.ViewMatchers.withId
import androidx.test.espresso.matcher.ViewMatchers.withText
import com.adevinta.android.barista.assertion.BaristaVisibilityAssertions
import com.adevinta.android.barista.interaction.BaristaClickInteractions.clickOn
import chat.pcp.app.R
import chat.pcp.app.espresso.tools.selectTabAtPosition
import chat.pcp.app.espresso.tools.waitUntilActivityVisible
import chat.pcp.app.espresso.tools.waitUntilDialogVisible
import chat.pcp.app.espresso.tools.waitUntilViewVisible
import chat.pcp.app.features.home.HomeActivity
import chat.pcp.app.features.home.room.list.home.header.HomeRoomFilter
import chat.pcp.app.features.roomdirectory.RoomDirectoryActivity
import chat.pcp.app.ui.robot.settings.labs.LabFeaturesPreferences
import chat.pcp.app.waitForView
import chat.pcp.lib.strings.CommonStrings

class RoomListRobot(private val labsPreferences: LabFeaturesPreferences) {

    fun openRoom(roomName: String, block: RoomDetailRobot.() -> Unit) {
        onView(withId(R.id.roomListView))
                .perform(
                        RecyclerViewActions.actionOnItem<RecyclerView.ViewHolder>(
                                hasDescendant(withText(roomName)),
                                ViewActions.click()
                        )
                )
        block(RoomDetailRobot())
        pressBack()
    }

    fun verifyCreatedRoom() {
        onView(withId(R.id.roomListView))
                .perform(
                        RecyclerViewActions.actionOnItem<RecyclerView.ViewHolder>(
                                hasDescendant(withText(CommonStrings.room_displayname_empty_room)),
                                ViewActions.longClick()
                        )
                )
        pressBack()
    }

    fun newRoom(block: NewRoomRobot.() -> Unit) {
        if (labsPreferences.isNewAppLayoutEnabled) {
            clickOn(R.id.newLayoutCreateChatButton)
            waitUntilDialogVisible(withId(R.id.create_room))
            clickOn(R.id.create_room)
        } else {
            clickOn(R.id.createGroupRoomButton)
            waitUntilActivityVisible<RoomDirectoryActivity> {
                BaristaVisibilityAssertions.assertDisplayed(R.id.publicRoomsList)
            }
        }
        val newRoomRobot = NewRoomRobot(false, labsPreferences)
        block(newRoomRobot)
        if (!newRoomRobot.createdRoom) {
            pressBack()
        }
    }

    fun crawlTabs() {
        waitUntilActivityVisible<HomeActivity> {
            waitUntilViewVisible(withId(R.id.roomListContainer))
        }

        selectFilterTab(HomeRoomFilter.UNREADS)
        waitForView(withId(R.id.emptyTitleView))
        selectFilterTab(HomeRoomFilter.ALL)
        waitForView(withId(R.id.roomNameView))
    }

    fun selectFilterTab(filter: HomeRoomFilter) {
        onView(withId(R.id.home_filter_tabs_tabs)).perform(selectTabAtPosition(filter.ordinal))
    }
}
