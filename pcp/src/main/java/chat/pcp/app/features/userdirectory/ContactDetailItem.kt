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
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.extensions.setTextOrHide

@EpoxyModelClass
abstract class ContactDetailItem : PcpEpoxyModel<ContactDetailItem.Holder>(R.layout.item_contact_detail) {

    @EpoxyAttribute lateinit var threePid: String
    @EpoxyAttribute var matrixId: String? = null
    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash) var clickListener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.view.onClick(clickListener)
        holder.nameView.text = threePid
        holder.matrixIdView.setTextOrHide(matrixId)
    }

    class Holder : PcpEpoxyHolder() {
        val nameView by bind<TextView>(R.id.contactDetailName)
        val matrixIdView by bind<TextView>(R.id.contactDetailMatrixId)
    }
}
