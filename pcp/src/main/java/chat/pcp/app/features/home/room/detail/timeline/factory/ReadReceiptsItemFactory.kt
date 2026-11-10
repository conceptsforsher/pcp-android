/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.factory

import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.app.features.home.room.detail.timeline.TimelineEventController
import chat.pcp.app.features.home.room.detail.timeline.item.ReadReceiptData
import chat.pcp.app.features.home.room.detail.timeline.item.ReadReceiptsItem
import chat.pcp.app.features.home.room.detail.timeline.item.ReadReceiptsItem_
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.room.model.ReadReceipt
import javax.inject.Inject

class ReadReceiptsItemFactory @Inject constructor(
        private val avatarRenderer: AvatarRenderer,
        private val session: Session
) {

    fun create(
            eventId: String,
            readReceipts: List<ReadReceipt>,
            callback: TimelineEventController.Callback?,
            isFromThreadTimeLine: Boolean,
    ): ReadReceiptsItem? {
        if (readReceipts.isEmpty()) {
            return null
        }
        val readReceiptsData = readReceipts
                .map {
                    ReadReceiptData(it.roomMember.userId, it.roomMember.avatarUrl, it.roomMember.displayName, it.originServerTs)
                }
                .sortedByDescending { it.timestamp }
        val threadReadReceiptsSupported = session.homeServerCapabilitiesService().getHomeServerCapabilities().canUseThreadReadReceiptsAndNotifications
        return ReadReceiptsItem_()
                .id("read_receipts_$eventId")
                .eventId(eventId)
                .readReceipts(readReceiptsData)
                .avatarRenderer(avatarRenderer)
                .shouldHideReadReceipts(isFromThreadTimeLine && !threadReadReceiptsSupported)
                .clickListener {
                    callback?.onReadReceiptsClicked(readReceiptsData)
                }
    }
}
