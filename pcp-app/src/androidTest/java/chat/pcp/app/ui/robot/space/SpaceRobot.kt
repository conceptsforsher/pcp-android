/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.ui.robot.space

import androidx.recyclerview.widget.RecyclerView
import androidx.test.espresso.Espresso
import androidx.test.espresso.action.ViewActions.click
import androidx.test.espresso.action.ViewActions.longClick
import androidx.test.espresso.contrib.RecyclerViewActions
import androidx.test.espresso.matcher.ViewMatchers
import com.adevinta.android.barista.interaction.BaristaClickInteractions.clickOn
import com.adevinta.android.barista.interaction.BaristaDrawerInteractions.openDrawer
import com.adevinta.android.barista.internal.viewaction.ClickChildAction
import chat.pcp.app.R
import chat.pcp.app.espresso.tools.waitUntilDialogVisible
import chat.pcp.app.espresso.tools.waitUntilViewVisible
import chat.pcp.app.features.DefaultPcpFeatures
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.ui.robot.settings.labs.LabFeaturesPreferences
import chat.pcp.lib.strings.CommonStrings
import org.hamcrest.Matchers

class SpaceRobot(private val labsPreferences: LabFeaturesPreferences) {
    private val features: PcpFeatures = DefaultPcpFeatures()

    fun createSpace(isFirstSpace: Boolean, block: SpaceCreateRobot.() -> Unit) {
        if (labsPreferences.isNewAppLayoutEnabled) {
            clickOn(R.id.newLayoutOpenSpacesButton)
            if (isFirstSpace) {
                waitUntilDialogVisible(ViewMatchers.withId(R.id.spaces_empty_group))
                clickOn(R.id.spaces_empty_button)
            } else {
                waitUntilDialogVisible(ViewMatchers.withId(R.id.groupListView))
                Espresso.onView(ViewMatchers.withId(R.id.groupListView))
                        .perform(
                                RecyclerViewActions.actionOnItem<RecyclerView.ViewHolder>(
                                        ViewMatchers.hasDescendant(ViewMatchers.withId(R.id.plus)),
                                        click()
                                ).atPosition(0)
                        )
            }
        } else {
            openDrawer()
            clickOn(CommonStrings.create_space)
        }
        block(SpaceCreateRobot())
    }

    fun spaceMenu(spaceName: String, block: SpaceMenuRobot.() -> Unit) {
        if (labsPreferences.isNewAppLayoutEnabled) {
            clickOn(R.id.newLayoutOpenSpacesButton)
            waitUntilDialogVisible(ViewMatchers.withId(R.id.groupListView))
        } else {
            openDrawer()
        }
        with(SpaceMenuRobot()) {
            openMenu(spaceName)
            block()
        }
    }

    fun openMenu(spaceName: String) {
        waitUntilViewVisible(ViewMatchers.withId(R.id.groupListView))
        if (labsPreferences.isNewAppLayoutEnabled) {
            Espresso.onView(ViewMatchers.withId(R.id.groupListView))
                    .perform(
                            RecyclerViewActions.actionOnItem<RecyclerView.ViewHolder>(
                                    ViewMatchers.hasDescendant(Matchers.allOf(ViewMatchers.withId(R.id.name), ViewMatchers.withText(spaceName))),
                                    longClick()
                            ).atPosition(0)
                    )
        } else {
            Espresso.onView(ViewMatchers.withId(R.id.groupListView))
                    .perform(
                            RecyclerViewActions.actionOnItem<RecyclerView.ViewHolder>(
                                    ViewMatchers.hasDescendant(Matchers.allOf(ViewMatchers.withId(R.id.groupNameView), ViewMatchers.withText(spaceName))),
                                    ClickChildAction.clickChildWithId(R.id.groupTmpLeave)
                            ).atPosition(0)
                    )
        }

        waitUntilDialogVisible(ViewMatchers.withId(R.id.spaceNameView))
    }

    fun selectSpace(spaceName: String) {
        if (!labsPreferences.isNewAppLayoutEnabled) {
            openDrawer()
            waitUntilViewVisible(ViewMatchers.withId(R.id.groupListView))
        }
        clickOn(spaceName)
    }
}
