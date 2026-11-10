/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.devtools

import com.airbnb.epoxy.EpoxyController
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.ui.list.genericButtonItem
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

class RoomDevToolRootController @Inject constructor(
        private val stringProvider: StringProvider
) : EpoxyController() {

    var interactionListener: DevToolsInteractionListener? = null

    init {
        requestModelBuild()
    }

    override fun buildModels() {
        val host = this
        genericButtonItem {
            id("explore")
            text(host.stringProvider.getString(CommonStrings.dev_tools_explore_room_state))
            buttonClickAction {
                host.interactionListener?.processAction(RoomDevToolAction.ExploreRoomState)
            }
        }
        genericButtonItem {
            id("send")
            text(host.stringProvider.getString(CommonStrings.dev_tools_send_custom_event))
            buttonClickAction {
                host.interactionListener?.processAction(RoomDevToolAction.SendCustomEvent(false))
            }
        }
        genericButtonItem {
            id("send_state")
            text(host.stringProvider.getString(CommonStrings.dev_tools_send_state_event))
            buttonClickAction {
                host.interactionListener?.processAction(RoomDevToolAction.SendCustomEvent(true))
            }
        }
    }
}
