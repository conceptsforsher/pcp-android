/*
 * Copyright 2023, 2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import android.content.Context
import chat.pcp.app.features.analytics.plan.ViewRoom
import chat.pcp.app.features.navigation.Navigator
import io.mockk.justRun
import io.mockk.mockk
import io.mockk.verify

class FakeNavigator {

    val instance: Navigator = mockk()

    fun givenOpenRoomSuccess(
            context: Context,
            roomId: String,
            eventId: String?,
            buildTask: Boolean,
            isInviteAlreadyAccepted: Boolean,
            trigger: ViewRoom.Trigger?,
    ) {
        justRun { instance.openRoom(context, roomId, eventId, buildTask, isInviteAlreadyAccepted, trigger) }
    }

    fun verifyOpenRoom(
            context: Context,
            roomId: String,
            eventId: String?,
            buildTask: Boolean,
            isInviteAlreadyAccepted: Boolean,
            trigger: ViewRoom.Trigger?,
    ) {
        verify { instance.openRoom(context, roomId, eventId, buildTask, isInviteAlreadyAccepted, trigger) }
    }
}
