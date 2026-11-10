/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.ui.robot

import com.adevinta.android.barista.interaction.BaristaClickInteractions.clickOn
import chat.pcp.app.R

class LayoutPreferencesRobot {

    fun crawl() {
        toggleRecents()
        toggleFilters()
        useAZOrderd()
        useActivityOrder()
    }

    fun toggleRecents() {
        clickOn(R.id.home_layout_settings_recents)
    }

    fun toggleFilters() {
        clickOn(R.id.home_layout_settings_filters)
    }

    fun useAZOrderd() {
        clickOn(R.id.home_layout_settings_sort_name)
    }

    fun useActivityOrder() {
        clickOn(R.id.home_layout_settings_sort_activity)
    }
}
