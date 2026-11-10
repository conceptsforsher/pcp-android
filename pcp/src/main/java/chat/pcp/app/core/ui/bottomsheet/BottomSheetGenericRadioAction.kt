/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.ui.bottomsheet

import chat.pcp.app.core.epoxy.bottomsheet.BottomSheetRadioActionItem_
import chat.pcp.app.core.platform.PcpSharedAction

/**
 * Parent class for a bottom sheet action.
 */
open class BottomSheetGenericRadioAction(
        open val title: String?,
        open val description: String? = null,
        open val isSelected: Boolean
) : PcpSharedAction {

    fun toRadioBottomSheetItem(): BottomSheetRadioActionItem_ {
        return BottomSheetRadioActionItem_().also {
            it.id("action_$title")
            it.title(title)
            it.selected(isSelected)
            it.description(description)
        }
    }
}
