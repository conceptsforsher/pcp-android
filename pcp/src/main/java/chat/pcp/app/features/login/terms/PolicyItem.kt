/*
 * Copyright 2018-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.login.terms

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
import chat.pcp.app.core.extensions.setHorizontalPadding

@EpoxyModelClass
abstract class PolicyItem : PcpEpoxyModel<PolicyItem.Holder>(R.layout.item_policy) {
    @EpoxyAttribute
    var checked: Boolean = false

    @EpoxyAttribute
    var title: String? = null

    @EpoxyAttribute
    var subtitle: String? = null

    @EpoxyAttribute
    var horizontalPadding: Int? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var checkChangeListener: CompoundButton.OnCheckedChangeListener? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var clickListener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        horizontalPadding?.let { holder.view.setHorizontalPadding(it) }
        holder.checkbox.isChecked = checked
        holder.checkbox.setOnCheckedChangeListener(checkChangeListener)
        holder.title.text = title
        holder.subtitle.text = subtitle
        holder.view.onClick(clickListener)
    }

    // Ensure checkbox behaves as expected (remove the listener)
    override fun unbind(holder: Holder) {
        super.unbind(holder)
        holder.checkbox.setOnCheckedChangeListener(null)
    }

    class Holder : PcpEpoxyHolder() {
        val checkbox by bind<CheckBox>(R.id.adapter_item_policy_checkbox)
        val title by bind<TextView>(R.id.adapter_item_policy_title)
        val subtitle by bind<TextView>(R.id.adapter_item_policy_subtitle)
    }
}
