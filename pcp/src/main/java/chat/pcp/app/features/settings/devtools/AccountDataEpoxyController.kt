/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devtools

import android.view.View
import com.airbnb.epoxy.TypedEpoxyController
import com.airbnb.mvrx.Fail
import com.airbnb.mvrx.Loading
import com.airbnb.mvrx.Success
import chat.pcp.app.core.epoxy.loadingItem
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.ui.list.genericFooterItem
import chat.pcp.app.core.ui.list.genericWithValueItem
import chat.pcp.lib.core.utils.epoxy.charsequence.toEpoxyCharSequence
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.session.accountdata.UserAccountDataEvent
import javax.inject.Inject

class AccountDataEpoxyController @Inject constructor(
        private val stringProvider: StringProvider
) : TypedEpoxyController<AccountDataViewState>() {

    interface InteractionListener {
        fun didTap(data: UserAccountDataEvent)
        fun didLongTap(data: UserAccountDataEvent)
    }

    var interactionListener: InteractionListener? = null

    override fun buildModels(data: AccountDataViewState?) {
        if (data == null) return
        val host = this
        when (data.accountData) {
            is Loading -> {
                loadingItem {
                    id("loading")
                    loadingText(host.stringProvider.getString(CommonStrings.loading))
                }
            }
            is Fail -> {
                genericFooterItem {
                    id("fail")
                    text(data.accountData.error.localizedMessage?.toEpoxyCharSequence())
                }
            }
            is Success -> {
                val dataList = data.accountData.invoke()
                if (dataList.isEmpty()) {
                    genericFooterItem {
                        id("noResults")
                        text(host.stringProvider.getString(CommonStrings.no_result_placeholder).toEpoxyCharSequence())
                    }
                } else {
                    dataList.forEach { accountData ->
                        genericWithValueItem {
                            id(accountData.type)
                            title(accountData.type.toEpoxyCharSequence())
                            itemClickAction {
                                host.interactionListener?.didTap(accountData)
                            }
                            itemLongClickAction(View.OnLongClickListener {
                                host.interactionListener?.didLongTap(accountData)
                                true
                            })
                        }
                    }
                }
            }
            else -> Unit
        }
    }
}
