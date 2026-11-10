/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.share

import chat.pcp.app.core.platform.PcpViewEvents
import org.matrix.android.sdk.api.session.content.ContentAttachmentData
import org.matrix.android.sdk.api.session.room.model.RoomSummary

sealed class IncomingShareViewEvents : PcpViewEvents {
    data class ShareToRoom(
            val roomSummary: RoomSummary,
            val sharedData: SharedData,
            val showAlert: Boolean
    ) : IncomingShareViewEvents()

    data class EditMediaBeforeSending(val contentAttachmentData: List<ContentAttachmentData>) : IncomingShareViewEvents()
    data class MultipleRoomsShareDone(val roomId: String) : IncomingShareViewEvents()
}
