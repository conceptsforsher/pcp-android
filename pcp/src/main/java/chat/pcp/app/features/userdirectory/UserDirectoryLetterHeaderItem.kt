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
abstract class UserDirectoryLetterHeaderItem : PcpEpoxyModel<UserDirectoryLetterHeaderItem.Holder>(R.layout.item_user_directory_letter_header) {

    @EpoxyAttribute var letter: String = ""

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.letterView.text = letter
    }

    class Holder : PcpEpoxyHolder() {
        val letterView by bind<TextView>(R.id.userDirectoryLetterView)
    }
}
