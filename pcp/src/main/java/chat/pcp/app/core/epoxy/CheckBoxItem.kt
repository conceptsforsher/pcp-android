/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy

import android.widget.CompoundButton
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import com.google.android.material.checkbox.MaterialCheckBox
import chat.pcp.app.R

@EpoxyModelClass
abstract class CheckBoxItem : PcpEpoxyModel<CheckBoxItem.Holder>(R.layout.item_checkbox) {

    @EpoxyAttribute
    var checked: Boolean = false

    @EpoxyAttribute lateinit var title: String

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var checkChangeListener: CompoundButton.OnCheckedChangeListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.checkbox.isChecked = checked
        holder.checkbox.text = title
        holder.checkbox.setOnCheckedChangeListener(checkChangeListener)
    }

    class Holder : PcpEpoxyHolder() {
        val checkbox by bind<MaterialCheckBox>(R.id.checkbox)
    }
}
