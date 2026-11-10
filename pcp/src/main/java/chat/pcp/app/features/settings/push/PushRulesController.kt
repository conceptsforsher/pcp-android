/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.push

import com.airbnb.epoxy.TypedEpoxyController
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.ui.list.genericFooterItem
import chat.pcp.lib.core.utils.epoxy.charsequence.toEpoxyCharSequence
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

class PushRulesController @Inject constructor(
        private val stringProvider: StringProvider
) : TypedEpoxyController<PushRulesViewState>() {

    override fun buildModels(data: PushRulesViewState?) {
        val host = this
        data?.let {
            it.rules.forEach {
                pushRuleItem {
                    id(it.ruleId)
                    pushRule(it)
                }
            }
        } ?: run {
            genericFooterItem {
                id("footer")
                text(host.stringProvider.getString(CommonStrings.settings_push_rules_no_rules).toEpoxyCharSequence())
            }
        }
    }
}
