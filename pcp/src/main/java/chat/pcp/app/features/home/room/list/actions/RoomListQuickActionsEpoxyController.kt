/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.home.room.list.actions

import androidx.annotation.StringRes
import com.airbnb.epoxy.TypedEpoxyController
import chat.pcp.app.core.epoxy.bottomSheetDividerItem
import chat.pcp.app.core.epoxy.bottomsheet.bottomSheetActionItem
import chat.pcp.app.core.epoxy.bottomsheet.bottomSheetRoomPreviewItem
import chat.pcp.app.core.epoxy.profiles.notifications.radioButtonItem
import chat.pcp.app.core.resources.ColorProvider
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.app.features.roomprofile.notifications.notificationOptions
import chat.pcp.app.features.roomprofile.notifications.notificationStateMapped
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.session.room.notification.RoomNotificationState
import org.matrix.android.sdk.api.util.toMatrixItem
import javax.inject.Inject

/**
 * Epoxy controller for room list actions.
 */
class RoomListQuickActionsEpoxyController @Inject constructor(
        private val avatarRenderer: AvatarRenderer,
        private val colorProvider: ColorProvider,
        private val stringProvider: StringProvider,
) : TypedEpoxyController<RoomListQuickActionViewState>() {

    var listener: Listener? = null

    override fun buildModels(state: RoomListQuickActionViewState) {
        val notificationViewState = state.notificationSettingsViewState
        val roomSummary = notificationViewState.roomSummary() ?: return
        val host = this
        // Preview, favorite, settings
        bottomSheetRoomPreviewItem {
            id("room_preview")
            avatarRenderer(host.avatarRenderer)
            matrixItem(roomSummary.toMatrixItem())
            stringProvider(host.stringProvider)
            colorProvider(host.colorProvider)
            izLowPriority(roomSummary.isLowPriority)
            izFavorite(roomSummary.isFavorite)
            settingsClickListener { host.listener?.didSelectMenuAction(RoomListQuickActionsSharedAction.Settings(roomSummary.roomId)) }
            favoriteClickListener { host.listener?.didSelectMenuAction(RoomListQuickActionsSharedAction.Favorite(roomSummary.roomId)) }
            lowPriorityClickListener { host.listener?.didSelectMenuAction(RoomListQuickActionsSharedAction.LowPriority(roomSummary.roomId)) }
        }

        // Notifications
        bottomSheetDividerItem {
            id("notifications_separator")
        }

        notificationViewState.notificationOptions.forEach { notificationState ->
            val title = titleForNotificationState(notificationState)
            radioButtonItem {
                id(notificationState.name)
                titleRes(title)
                selected(notificationViewState.notificationStateMapped() == notificationState)
                listener {
                    host.listener?.didSelectRoomNotificationState(notificationState)
                }
            }
        }

        RoomListQuickActionsSharedAction.Leave(roomSummary.roomId, showIcon = !true).toBottomSheetItem()
    }

    @StringRes
    private fun titleForNotificationState(notificationState: RoomNotificationState): Int? = when (notificationState) {
        RoomNotificationState.ALL_MESSAGES_NOISY -> CommonStrings.room_settings_all_messages
        RoomNotificationState.MENTIONS_ONLY -> CommonStrings.room_settings_mention_and_keyword_only
        RoomNotificationState.MUTE -> CommonStrings.room_settings_none
        else -> null
    }

    private fun RoomListQuickActionsSharedAction.Leave.toBottomSheetItem() {
        val host = this@RoomListQuickActionsEpoxyController
        return bottomSheetActionItem {
            id("action_leave")
            selected(false)
            if (iconResId != null) {
                iconRes(iconResId)
            } else {
                showIcon(false)
            }
            textRes(titleRes)
            destructive(this@toBottomSheetItem.destructive)
            listener { host.listener?.didSelectMenuAction(this@toBottomSheetItem) }
        }
    }

    interface Listener {
        fun didSelectMenuAction(quickAction: RoomListQuickActionsSharedAction)
        fun didSelectRoomNotificationState(roomNotificationState: RoomNotificationState)
    }
}
