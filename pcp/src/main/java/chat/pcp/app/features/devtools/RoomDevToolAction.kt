/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.devtools

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.events.model.Event

sealed class RoomDevToolAction : PcpViewModelAction {
    object ExploreRoomState : RoomDevToolAction()
    object OnBackPressed : RoomDevToolAction()
    object MenuEdit : RoomDevToolAction()
    object MenuItemSend : RoomDevToolAction()
    data class ShowStateEvent(val event: Event) : RoomDevToolAction()
    data class ShowStateEventType(val stateEventType: String) : RoomDevToolAction()
    data class UpdateContentText(val contentJson: String) : RoomDevToolAction()
    data class SendCustomEvent(val isStateEvent: Boolean) : RoomDevToolAction()
    data class CustomEventTypeChange(val type: String) : RoomDevToolAction()
    data class CustomEventContentChange(val content: String) : RoomDevToolAction()
    data class CustomEventStateKeyChange(val stateKey: String) : RoomDevToolAction()
}
