/*
 * Copyright 2026, 2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls.detail.ui

import android.content.Context
import chat.pcp.app.features.navigation.Navigator
import javax.inject.Inject

class RoomPollDetailNavigator @Inject constructor(
        private val navigator: Navigator,
) {

    fun goToTimelineEvent(context: Context, roomId: String, eventId: String) {
        navigator.openRoom(
                context = context,
                roomId = roomId,
                eventId = eventId,
                buildTask = true,
        )
    }
}
