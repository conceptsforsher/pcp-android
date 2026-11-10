/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy

import android.widget.CheckBox
import android.widget.CompoundButton
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R

@EpoxyModelClass
abstract class FontScaleUseSystemSettingsItem : PcpEpoxyModel<FontScaleUseSystemSettingsItem.Holder>(R.layout.item_font_scale_system) {

    @EpoxyAttribute var useSystemSettings: Boolean = true

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var checkChangeListener: CompoundButton.OnCheckedChangeListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.checkBox.isChecked = useSystemSettings
        holder.checkBox.setOnCheckedChangeListener(checkChangeListener)
        holder.view.onClick {
            holder.checkBox.performClick()
        }
    }

    class Holder : PcpEpoxyHolder() {
        val checkBox by bind<CheckBox>(R.id.font_scale_use_system_checkbox)
    }
}
