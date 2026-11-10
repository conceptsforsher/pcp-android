/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.factory

import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.resources.UserPreferencesProvider
import chat.pcp.app.features.home.room.detail.timeline.item.RoomCreateItem_
import chat.pcp.lib.core.utils.epoxy.charsequence.toEpoxyCharSequence
import chat.pcp.lib.strings.CommonStrings
import me.gujun.android.span.span
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.events.model.toModel
import org.matrix.android.sdk.api.session.room.model.create.RoomCreateContent
import javax.inject.Inject

class RoomCreateItemFactory @Inject constructor(
        private val stringProvider: StringProvider,
        private val userPreferencesProvider: UserPreferencesProvider,
        private val session: Session,
        private val noticeItemFactory: NoticeItemFactory
) {

    fun create(params: TimelineItemFactoryParams): PcpEpoxyModel<*>? {
        val event = params.event
        val createRoomContent = event.root.content.toModel<RoomCreateContent>() ?: return null
        val predecessorId = createRoomContent.predecessor?.roomId ?: return defaultRendering(params)
        val roomLink = session.permalinkService().createRoomPermalink(predecessorId) ?: return null
        val text = span {
            +stringProvider.getString(CommonStrings.room_tombstone_continuation_description)
            +"\n"
            span(stringProvider.getString(CommonStrings.room_tombstone_predecessor_link)) {
                textDecorationLine = "underline"
                onClick = { params.callback?.onRoomCreateLinkClicked(roomLink) }
            }
        }
        return RoomCreateItem_()
                .text(text.toEpoxyCharSequence())
    }

    private fun defaultRendering(params: TimelineItemFactoryParams): PcpEpoxyModel<*>? {
        return if (userPreferencesProvider.shouldShowHiddenEvents()) {
            noticeItemFactory.create(params)
        } else {
            null
        }
    }
}
