/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.action

import androidx.recyclerview.widget.DefaultItemAnimator

private const val ANIM_DURATION_IN_MILLIS = 300L

/**
 * We only want to animate the expand of the "Report content" submenu.
 */
class MessageActionsAnimator : DefaultItemAnimator() {

    init {
        addDuration = ANIM_DURATION_IN_MILLIS
        removeDuration = 0
        moveDuration = 0
        changeDuration = 0
    }
}
