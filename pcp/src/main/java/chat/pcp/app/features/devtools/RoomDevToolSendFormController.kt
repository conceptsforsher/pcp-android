/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.devtools

import com.airbnb.epoxy.TypedEpoxyController
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.ui.list.genericFooterItem
import chat.pcp.app.features.form.formEditTextItem
import chat.pcp.app.features.form.formMultiLineEditTextItem
import chat.pcp.lib.core.utils.epoxy.charsequence.toEpoxyCharSequence
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

class RoomDevToolSendFormController @Inject constructor(
        private val stringProvider: StringProvider
) : TypedEpoxyController<RoomDevToolViewState>() {

    var interactionListener: DevToolsInteractionListener? = null

    override fun buildModels(data: RoomDevToolViewState?) {
        val sendEventForm = (data?.displayMode as? RoomDevToolViewState.Mode.SendEventForm) ?: return
        val host = this

        genericFooterItem {
            id("topSpace")
            text("".toEpoxyCharSequence())
        }
        formEditTextItem {
            id("event_type")
            enabled(true)
            value(data.sendEventDraft?.type)
            hint(host.stringProvider.getString(CommonStrings.dev_tools_form_hint_type))
            onTextChange { text ->
                host.interactionListener?.processAction(RoomDevToolAction.CustomEventTypeChange(text))
            }
        }

        if (sendEventForm.isState) {
            formEditTextItem {
                id("state_key")
                enabled(true)
                value(data.sendEventDraft?.stateKey)
                hint(host.stringProvider.getString(CommonStrings.dev_tools_form_hint_state_key))
                onTextChange { text ->
                    host.interactionListener?.processAction(RoomDevToolAction.CustomEventStateKeyChange(text))
                }
            }
        }

        formMultiLineEditTextItem {
            id("event_content")
            enabled(true)
            value(data.sendEventDraft?.content)
            hint(host.stringProvider.getString(CommonStrings.dev_tools_form_hint_event_content))
            onTextChange { text ->
                host.interactionListener?.processAction(RoomDevToolAction.CustomEventContentChange(text))
            }
        }
    }
}
