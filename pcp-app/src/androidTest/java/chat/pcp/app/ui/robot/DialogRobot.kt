/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.ui.robot

import com.adevinta.android.barista.interaction.BaristaDialogInteractions.clickDialogNegativeButton

class DialogRobot(
        var returnedToPreviousScreen: Boolean = false
) {

    fun negativeAction() {
        clickDialogNegativeButton()
        returnedToPreviousScreen = true
    }
}
