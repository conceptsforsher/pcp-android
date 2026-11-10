/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import chat.pcp.app.features.settings.devices.v2.notification.GetNotificationsStatusUseCase
import chat.pcp.app.features.settings.devices.v2.notification.NotificationsStatus
import io.mockk.every
import io.mockk.mockk
import kotlinx.coroutines.flow.flowOf
import org.matrix.android.sdk.api.session.Session

class FakeGetNotificationsStatusUseCase {

    val instance = mockk<GetNotificationsStatusUseCase>()

    fun givenExecuteReturns(
            session: Session,
            sessionId: String,
            notificationsStatus: NotificationsStatus
    ) {
        every { instance.execute(session, sessionId) } returns flowOf(notificationsStatus)
    }
}
