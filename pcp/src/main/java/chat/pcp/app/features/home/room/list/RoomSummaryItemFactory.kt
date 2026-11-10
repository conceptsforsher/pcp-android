/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list

import com.airbnb.mvrx.Async
import com.airbnb.mvrx.Fail
import com.airbnb.mvrx.Loading
import chat.pcp.app.core.date.DateFormatKind
import chat.pcp.app.core.date.PcpDateFormatter
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.error.ErrorFormatter
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.app.features.home.RoomListDisplayMode
import chat.pcp.app.features.home.room.detail.timeline.format.DisplayableEventFormatter
import chat.pcp.app.features.home.room.list.usecase.GetLatestPreviewableEventUseCase
import chat.pcp.app.features.home.room.typing.TypingHelper
import chat.pcp.app.features.voicebroadcast.isLive
import chat.pcp.app.features.voicebroadcast.model.asVoiceBroadcastEvent
import chat.pcp.lib.core.utils.epoxy.charsequence.toEpoxyCharSequence
import chat.pcp.lib.strings.CommonPlurals
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.extensions.orFalse
import org.matrix.android.sdk.api.session.room.members.ChangeMembershipState
import org.matrix.android.sdk.api.session.room.model.Membership
import org.matrix.android.sdk.api.session.room.model.RoomSummary
import org.matrix.android.sdk.api.session.room.model.SpaceChildInfo
import org.matrix.android.sdk.api.util.toMatrixItem
import javax.inject.Inject

class RoomSummaryItemFactory @Inject constructor(
        private val displayableEventFormatter: DisplayableEventFormatter,
        private val dateFormatter: PcpDateFormatter,
        private val stringProvider: StringProvider,
        private val typingHelper: TypingHelper,
        private val avatarRenderer: AvatarRenderer,
        private val errorFormatter: ErrorFormatter,
        private val getLatestPreviewableEventUseCase: GetLatestPreviewableEventUseCase,
) {

    fun create(
            roomSummary: RoomSummary,
            roomChangeMembershipStates: Map<String, ChangeMembershipState>,
            selectedRoomIds: Set<String>,
            displayMode: RoomListDisplayMode,
            listener: RoomListListener?,
            singleLineLastEvent: Boolean = false
    ): PcpEpoxyModel<*> {
        return when (roomSummary.membership) {
            Membership.INVITE -> {
                val changeMembershipState = roomChangeMembershipStates[roomSummary.roomId] ?: ChangeMembershipState.Unknown
                createInvitationItem(roomSummary, changeMembershipState, listener)
            }
            else -> createRoomItem(
                    roomSummary, selectedRoomIds, displayMode, singleLineLastEvent, listener?.let { it::onRoomClicked }, listener?.let { it::onRoomLongClicked }
            )
        }
    }

    fun createSuggestion(
            spaceChildInfo: SpaceChildInfo,
            suggestedRoomJoiningStates: Map<String, Async<Unit>>,
            listener: RoomListListener?
    ): PcpEpoxyModel<*> {
        val error = (suggestedRoomJoiningStates[spaceChildInfo.childRoomId] as? Fail)?.error
        return SpaceChildInfoItem_()
                .id("sug_${spaceChildInfo.childRoomId}")
                .matrixItem(spaceChildInfo.toMatrixItem())
                .avatarRenderer(avatarRenderer)
                .topic(spaceChildInfo.topic)
                .errorLabel(
                        error?.let {
                            stringProvider.getString(CommonStrings.error_failed_to_join_room, errorFormatter.toHumanReadable(it))
                        }
                )
                .buttonLabel(
                        if (error != null) stringProvider.getString(CommonStrings.global_retry)
                        else stringProvider.getString(CommonStrings.action_join)
                )
                .loading(suggestedRoomJoiningStates[spaceChildInfo.childRoomId] is Loading)
                .memberCount(spaceChildInfo.activeMemberCount ?: 0)
                .buttonClickListener { listener?.onJoinSuggestedRoom(spaceChildInfo) }
                .itemClickListener { listener?.onSuggestedRoomClicked(spaceChildInfo) }
    }

    private fun createInvitationItem(
            roomSummary: RoomSummary,
            changeMembershipState: ChangeMembershipState,
            listener: RoomListListener?
    ): PcpEpoxyModel<*> {
        val secondLine = if (roomSummary.isDirect) {
            roomSummary.inviterId
        } else {
            roomSummary.inviterId?.let {
                stringProvider.getString(CommonStrings.invited_by, it)
            }
        }

        return RoomInvitationItem_()
                .id(roomSummary.roomId)
                .avatarRenderer(avatarRenderer)
                .matrixItem(roomSummary.toMatrixItem())
                .secondLine(secondLine)
                .changeMembershipState(changeMembershipState)
                .acceptListener { listener?.onAcceptRoomInvitation(roomSummary) }
                .rejectListener { listener?.onRejectRoomInvitation(roomSummary) }
                .listener { listener?.onRoomClicked(roomSummary) }
    }

    fun createRoomItem(
            roomSummary: RoomSummary,
            selectedRoomIds: Set<String>,
            displayMode: RoomListDisplayMode,
            singleLineLastEvent: Boolean,
            onClick: ((RoomSummary) -> Unit)?,
            onLongClick: ((RoomSummary) -> Boolean)?,
    ): PcpEpoxyModel<*> {
        val subtitle = getSearchResultSubtitle(roomSummary)
        val unreadCount = roomSummary.notificationCount
        val showHighlighted = roomSummary.highlightCount > 0
        val showSelected = selectedRoomIds.contains(roomSummary.roomId)
        var latestFormattedEvent: CharSequence = ""
        var latestEventTime = ""
        val latestEvent = getLatestPreviewableEventUseCase.execute(roomSummary.roomId)
        if (latestEvent != null) {
            latestFormattedEvent = displayableEventFormatter.format(latestEvent, roomSummary.isDirect, roomSummary.isDirect.not())
            latestEventTime = dateFormatter.format(latestEvent.root.originServerTs, DateFormatKind.ROOM_LIST)
        }

        val typingMessage = typingHelper.getTypingMessage(roomSummary.typingUsers)
                // Skip typing while there is a live voice broadcast
                .takeUnless { latestEvent?.root?.asVoiceBroadcastEvent()?.isLive.orFalse() }
                .orEmpty()

        return if (subtitle.isBlank() && displayMode == RoomListDisplayMode.FILTERED) {
            createCenteredRoomSummaryItem(roomSummary, displayMode, showSelected, unreadCount, onClick, onLongClick)
        } else {
            createRoomSummaryItem(
                    roomSummary, displayMode, subtitle, latestEventTime, typingMessage,
                    latestFormattedEvent, showHighlighted, showSelected, unreadCount, singleLineLastEvent, onClick, onLongClick
            )
        }
    }

    private fun createRoomSummaryItem(
            roomSummary: RoomSummary,
            displayMode: RoomListDisplayMode,
            subtitle: String,
            latestEventTime: String,
            typingMessage: String,
            latestFormattedEvent: CharSequence,
            showHighlighted: Boolean,
            showSelected: Boolean,
            unreadCount: Int,
            singleLineLastEvent: Boolean,
            onClick: ((RoomSummary) -> Unit)?,
            onLongClick: ((RoomSummary) -> Boolean)?
    ) = RoomSummaryItem_()
            .id(roomSummary.roomId)
            .avatarRenderer(avatarRenderer)
            // We do not display shield in the room list anymore
            // .encryptionTrustLevel(roomSummary.roomEncryptionTrustLevel)
            .displayMode(displayMode)
            .subtitle(subtitle)
            .izPublic(roomSummary.isPublic)
            .showPresence(roomSummary.isDirect)
            .userPresence(roomSummary.directUserPresence)
            .matrixItem(roomSummary.toMatrixItem())
            .lastEventTime(latestEventTime)
            .typingMessage(typingMessage)
            .lastFormattedEvent(latestFormattedEvent.toEpoxyCharSequence())
            .showHighlighted(showHighlighted)
            .showSelected(showSelected)
            .hasFailedSending(roomSummary.hasFailedSending)
            .unreadNotificationCount(unreadCount)
            .hasUnreadMessage(roomSummary.hasUnreadMessages)
            .hasDraft(roomSummary.userDrafts.isNotEmpty())
            .useSingleLineForLastEvent(singleLineLastEvent)
            .itemLongClickListener { _ -> onLongClick?.invoke(roomSummary) ?: false }
            .itemClickListener { onClick?.invoke(roomSummary) }

    private fun createCenteredRoomSummaryItem(
            roomSummary: RoomSummary,
            displayMode: RoomListDisplayMode,
            showSelected: Boolean,
            unreadCount: Int,
            onClick: ((RoomSummary) -> Unit)?,
            onLongClick: ((RoomSummary) -> Boolean)?
    ) = RoomSummaryCenteredItem_()
            .id(roomSummary.roomId)
            .avatarRenderer(avatarRenderer)
            // We do not display shield in the room list anymore
            // .encryptionTrustLevel(roomSummary.roomEncryptionTrustLevel)
            .displayMode(displayMode)
            .izPublic(roomSummary.isPublic)
            .showPresence(roomSummary.isDirect)
            .userPresence(roomSummary.directUserPresence)
            .matrixItem(roomSummary.toMatrixItem())
            .showSelected(showSelected)
            .hasFailedSending(roomSummary.hasFailedSending)
            .unreadNotificationCount(unreadCount)
            .hasUnreadMessage(roomSummary.hasUnreadMessages)
            .hasDraft(roomSummary.userDrafts.isNotEmpty())
            .itemLongClickListener { _ -> onLongClick?.invoke(roomSummary) ?: false }
            .itemClickListener { onClick?.invoke(roomSummary) }

    private fun getSearchResultSubtitle(roomSummary: RoomSummary): String {
        val userId = roomSummary.directUserId
        val directParent = joinParentNames(roomSummary)
        val canonicalAlias = roomSummary.canonicalAlias

        return (userId ?: directParent ?: canonicalAlias).orEmpty()
    }

    private fun joinParentNames(roomSummary: RoomSummary) = with(roomSummary) {
        when (val size = directParentNames.size) {
            0 -> null
            1 -> directParentNames.first()
            2 -> stringProvider.getString(CommonStrings.search_space_two_parents, directParentNames[0], directParentNames[1])
            else -> stringProvider.getQuantityString(CommonPlurals.search_space_multiple_parents, size - 1, directParentNames[0], size - 1)
        }
    }
}
