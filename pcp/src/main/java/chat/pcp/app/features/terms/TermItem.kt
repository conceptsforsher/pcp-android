/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.terms

import android.widget.CheckBox
import android.widget.CompoundButton
import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick

@EpoxyModelClass
abstract class TermItem : PcpEpoxyModel<TermItem.Holder>(R.layout.item_tos) {

    @EpoxyAttribute
    var checked: Boolean = false

    @EpoxyAttribute
    var name: String? = null

    @EpoxyAttribute
    var description: String? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var checkChangeListener: CompoundButton.OnCheckedChangeListener? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var clickListener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.checkbox.isChecked = checked
        holder.title.text = name
        holder.description.text = description
        holder.checkbox.setOnCheckedChangeListener(checkChangeListener)
        holder.view.onClick(clickListener)
    }

    class Holder : PcpEpoxyHolder() {
        val checkbox by bind<CheckBox>(R.id.term_accept_checkbox)
        val title by bind<TextView>(R.id.term_name)
        val description by bind<TextView>(R.id.term_description)
    }
}
