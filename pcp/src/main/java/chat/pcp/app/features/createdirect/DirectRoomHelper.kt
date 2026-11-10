/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.createdirect

import chat.pcp.app.features.analytics.AnalyticsTracker
import im.vector.app.features.analytics.plan.CreatedRoom
import chat.pcp.app.features.raw.wellknown.getElementWellknown
import chat.pcp.app.features.raw.wellknown.isE2EByDefault
import chat.pcp.app.features.settings.PcpBasePreferences
import org.matrix.android.sdk.api.extensions.orFalse
import org.matrix.android.sdk.api.extensions.tryOrNull
import org.matrix.android.sdk.api.raw.RawService
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.room.model.create.CreateRoomParams
import javax.inject.Inject

class DirectRoomHelper @Inject constructor(
        private val rawService: RawService,
        private val session: Session,
        private val analyticsTracker: AnalyticsTracker,
        private val pcpPreferences: PcpBasePreferences,
) {

    suspend fun ensureDMExists(userId: String): String {
        val existingRoomId = tryOrNull { session.roomService().getExistingDirectRoomWithUser(userId) }
        val roomId: String
        if (existingRoomId != null) {
            roomId = existingRoomId
        } else {
            val adminE2EByDefault = rawService.getElementWellknown(session.sessionParams)
                    ?.isE2EByDefault()
                    ?: true

            val roomParams = CreateRoomParams().apply {
                invitedUserIds.add(userId)
                setDirectMessage()
                enableEncryptionIfInvitedUsersSupportIt = adminE2EByDefault
            }
            roomId = if (pcpPreferences.isDeferredDmEnabled()) {
                session.roomService().createLocalRoom(roomParams)
            } else {
                analyticsTracker.capture(CreatedRoom(isDM = roomParams.isDirect.orFalse()))
                session.roomService().createRoom(roomParams)
            }
        }
        return roomId
    }
}
