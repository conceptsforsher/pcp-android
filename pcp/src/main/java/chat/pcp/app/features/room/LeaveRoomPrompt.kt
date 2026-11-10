/*
 * Copyright 2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.room

import android.content.Context
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import chat.pcp.app.features.powerlevel.isLastAdminFlow
import chat.pcp.app.features.room.LeaveRoomPrompt.Warning
import chat.pcp.lib.strings.CommonStrings
import chat.pcp.lib.ui.styles.R
import kotlinx.coroutines.flow.first
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.getRoom
import org.matrix.android.sdk.api.session.room.state.isPublic

object LeaveRoomPrompt {

    enum class Warning {
        LAST_ADMIN,
        PRIVATE_ROOM,
        NONE
    }

    fun show(
            context: Context,
            warning: Warning,
            onLeaveClick: () -> Unit
    ) {
        val hasWarning = warning != Warning.NONE
        val message = buildString {
            append(context.getString(CommonStrings.room_participants_leave_prompt_msg))
            if (hasWarning) append("\n\n")
            when (warning) {
                Warning.LAST_ADMIN -> append(context.getString(CommonStrings.room_participants_leave_last_admin))
                Warning.PRIVATE_ROOM -> append(context.getString(CommonStrings.room_participants_leave_private_warning))
                Warning.NONE -> Unit
            }
        }
        MaterialAlertDialogBuilder(
                context,
                if (hasWarning) R.style.ThemeOverlay_Vector_MaterialAlertDialog_Destructive else 0
        )
                .setTitle(CommonStrings.room_participants_leave_prompt_title)
                .setMessage(message)
                .setPositiveButton(CommonStrings.action_leave) { _, _ ->
                    onLeaveClick()
                }
                .setNegativeButton(CommonStrings.action_cancel, null)
                .show()
    }
}

suspend fun Session.getLeaveRoomWarning(roomId: String): Warning {
    val room = getRoom(roomId) ?: return Warning.NONE
    val isLastAdmin = room.isLastAdminFlow(myUserId).first()
    return when {
        isLastAdmin -> Warning.LAST_ADMIN
        !room.stateService().isPublic() -> Warning.PRIVATE_ROOM
        else -> Warning.NONE
    }
}
