/*
 * Copyright 2026, 2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls.list.ui

import android.content.Context
import chat.pcp.app.features.roomprofile.polls.detail.ui.RoomPollDetailActivity
import javax.inject.Inject

class RoomPollsListNavigator @Inject constructor() {

    fun goToPollDetails(context: Context, pollId: String, roomId: String, isEnded: Boolean) {
        context.startActivity(
                RoomPollDetailActivity.newIntent(
                        context = context,
                        pollId = pollId,
                        roomId = roomId,
                        isEnded = isEnded,
                )
        )
    }
}
