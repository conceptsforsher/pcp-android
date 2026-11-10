/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.userdirectory

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel

@EpoxyModelClass
abstract class UserListHeaderItem : PcpEpoxyModel<UserListHeaderItem.Holder>(R.layout.item_user_list_header) {

    @EpoxyAttribute var header: String = ""

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.headerTextView.text = header
    }

    class Holder : PcpEpoxyHolder() {
        val headerTextView by bind<TextView>(R.id.userListHeaderView)
    }
}
