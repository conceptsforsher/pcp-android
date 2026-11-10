/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.media

import chat.pcp.app.core.date.PcpDateFormatter
import chat.pcp.app.core.resources.StringProvider
import kotlinx.coroutines.CoroutineScope
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.room.Room
import org.matrix.android.sdk.api.session.room.timeline.TimelineEvent
import javax.inject.Inject

class AttachmentProviderFactory @Inject constructor(
        private val imageContentRenderer: ImageContentRenderer,
        private val vectorDateFormatter: PcpDateFormatter,
        private val stringProvider: StringProvider,
        private val session: Session
) {

    fun createProvider(
            attachments: List<TimelineEvent>,
            coroutineScope: CoroutineScope
    ): RoomEventsAttachmentProvider {
        return RoomEventsAttachmentProvider(
                attachments = attachments,
                imageContentRenderer = imageContentRenderer,
                dateFormatter = vectorDateFormatter,
                fileService = session.fileService(),
                coroutineScope = coroutineScope,
                stringProvider = stringProvider
        )
    }

    fun createProvider(
            attachments: List<AttachmentData>,
            room: Room?,
            coroutineScope: CoroutineScope
    ): DataAttachmentRoomProvider {
        return DataAttachmentRoomProvider(
                attachments = attachments,
                room = room,
                imageContentRenderer = imageContentRenderer,
                dateFormatter = vectorDateFormatter,
                fileService = session.fileService(),
                coroutineScope = coroutineScope,
                stringProvider = stringProvider
        )
    }
}
