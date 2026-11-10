/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.reactions

import com.airbnb.epoxy.TypedEpoxyController
import com.airbnb.mvrx.Fail
import com.airbnb.mvrx.Loading
import com.airbnb.mvrx.Success
import com.airbnb.mvrx.Uninitialized
import chat.pcp.app.EmojiSpanify
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.ui.list.genericFooterItem
import chat.pcp.app.core.ui.list.genericLoaderItem
import chat.pcp.lib.core.utils.epoxy.charsequence.toEpoxyCharSequence
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

/**
 * Epoxy controller for reaction event list.
 */
class ViewReactionsEpoxyController @Inject constructor(
        private val stringProvider: StringProvider,
        private val emojiSpanify: EmojiSpanify
) :
        TypedEpoxyController<DisplayReactionsViewState>() {

    var listener: Listener? = null

    override fun buildModels(state: DisplayReactionsViewState) {
        val host = this
        when (state.mapReactionKeyToMemberList) {
            Uninitialized,
            is Loading -> {
                genericLoaderItem {
                    id("Spinner")
                }
            }
            is Fail -> {
                genericFooterItem {
                    id("failure")
                    text(host.stringProvider.getString(CommonStrings.unknown_error).toEpoxyCharSequence())
                }
            }
            is Success -> {
                state.mapReactionKeyToMemberList()?.forEach { reactionInfo ->
                    reactionInfoSimpleItem {
                        id(reactionInfo.eventId)
                        timeStamp(reactionInfo.timestamp)
                        reactionKey(host.emojiSpanify.spanify(reactionInfo.reactionKey).toEpoxyCharSequence())
                        authorDisplayName(reactionInfo.authorName ?: reactionInfo.authorId)
                        userClicked { host.listener?.didSelectUser(reactionInfo.authorId) }
                    }
                }
            }
        }
    }

    interface Listener {
        fun didSelectUser(userId: String)
    }
}
