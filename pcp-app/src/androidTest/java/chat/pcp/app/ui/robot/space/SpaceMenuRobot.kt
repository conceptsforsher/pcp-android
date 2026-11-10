/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.ui.robot.space

import androidx.test.espresso.Espresso
import androidx.test.espresso.matcher.ViewMatchers
import com.adevinta.android.barista.interaction.BaristaClickInteractions.clickOn
import chat.pcp.app.R
import chat.pcp.app.clickOnSheet
import chat.pcp.app.espresso.tools.waitUntilActivityVisible
import chat.pcp.app.espresso.tools.waitUntilDialogVisible
import chat.pcp.app.espresso.tools.waitUntilViewVisible
import chat.pcp.app.features.invite.InviteUsersToRoomActivity
import chat.pcp.app.features.roomprofile.RoomProfileActivity
import chat.pcp.app.features.spaces.SpaceExploreActivity
import chat.pcp.app.features.spaces.leave.SpaceLeaveAdvancedActivity
import chat.pcp.app.features.spaces.manage.SpaceManageActivity

class SpaceMenuRobot {

    fun invitePeople() = apply {
        clickOnSheet(R.id.invitePeople)
        waitUntilDialogVisible(ViewMatchers.withId(R.id.inviteByMxidButton))
        clickOn(R.id.inviteByMxidButton)
        waitUntilActivityVisible<InviteUsersToRoomActivity> {
            waitUntilViewVisible(ViewMatchers.withId(R.id.userListRecyclerView))
        }
        // close keyboard
        Espresso.pressBack()
        // close invite view
        Espresso.pressBack()
    }

    fun spaceMembers() {
        clickOnSheet(R.id.showMemberList)
        waitUntilActivityVisible<RoomProfileActivity> {
            waitUntilViewVisible(ViewMatchers.withId(R.id.roomSettingsRecyclerView))
        }
        Espresso.pressBack()
    }

    fun spaceSettings(block: SpaceSettingsRobot.() -> Unit) {
        clickOnSheet(R.id.spaceSettings)
        waitUntilActivityVisible<SpaceManageActivity> {
            waitUntilViewVisible(ViewMatchers.withId(R.id.roomSettingsRecyclerView))
        }
        block(SpaceSettingsRobot())
    }

    fun exploreRooms() {
        clickOnSheet(R.id.exploreRooms)
        waitUntilActivityVisible<SpaceExploreActivity> {
            waitUntilViewVisible(ViewMatchers.withId(R.id.spaceDirectoryList))
        }
        Espresso.pressBack()
    }

    fun addRoom() = apply {
        clickOnSheet(R.id.addRooms)
        waitUntilActivityVisible<SpaceManageActivity> {
            waitUntilViewVisible(ViewMatchers.withId(R.id.roomList))
        }
        Espresso.pressBack()
    }

    fun addSpace() = apply {
        clickOnSheet(R.id.addSpaces)
        waitUntilActivityVisible<SpaceManageActivity> {
            waitUntilViewVisible(ViewMatchers.withId(R.id.roomList))
        }
        Espresso.pressBack()
    }

    fun leaveSpace() {
        clickOnSheet(R.id.leaveSpace)
        waitUntilActivityVisible<SpaceLeaveAdvancedActivity> {
            waitUntilViewVisible(ViewMatchers.withId(R.id.roomList))
            clickOn(R.id.spaceLeaveSelectAll)
            clickOn(R.id.spaceLeaveButton)
        }
    }
}
