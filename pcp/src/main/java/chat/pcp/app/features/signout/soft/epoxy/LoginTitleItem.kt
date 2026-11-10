/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.signout.soft.epoxy

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.extensions.setTextOrHide

@EpoxyModelClass
abstract class LoginTitleItem : PcpEpoxyModel<LoginTitleItem.Holder>(R.layout.item_login_title) {

    @EpoxyAttribute var text: String? = null

    override fun bind(holder: Holder) {
        super.bind(holder)

        holder.textView.setTextOrHide(text)
    }

    class Holder : PcpEpoxyHolder() {
        val textView by bind<TextView>(R.id.itemLoginTitleText)
    }
}
