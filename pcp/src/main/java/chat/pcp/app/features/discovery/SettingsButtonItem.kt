/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.discovery

import android.widget.Button
import androidx.annotation.StringRes
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.attributes.ButtonStyle
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.extensions.setTextOrHide
import chat.pcp.app.core.resources.ColorProvider

@EpoxyModelClass
abstract class SettingsButtonItem : PcpEpoxyModel<SettingsButtonItem.Holder>(R.layout.item_settings_button) {

    @EpoxyAttribute
    lateinit var colorProvider: ColorProvider

    @EpoxyAttribute
    var buttonTitle: String? = null

    @EpoxyAttribute
    @StringRes
    var buttonTitleId: Int? = null

    @EpoxyAttribute
    var buttonStyle: ButtonStyle = ButtonStyle.POSITIVE

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var buttonClickListener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        if (buttonTitleId != null) {
            holder.button.setText(buttonTitleId!!)
        } else {
            holder.button.setTextOrHide(buttonTitle)
        }

        when (buttonStyle) {
            ButtonStyle.POSITIVE -> {
                holder.button.setTextColor(colorProvider.getColorFromAttribute(com.google.android.material.R.attr.colorPrimary))
            }
            ButtonStyle.DESTRUCTIVE -> {
                holder.button.setTextColor(colorProvider.getColorFromAttribute(com.google.android.material.R.attr.colorError))
            }
        }

        holder.button.onClick(buttonClickListener)
    }

    class Holder : PcpEpoxyHolder() {
        val button by bind<Button>(R.id.settings_item_button)
    }
}
