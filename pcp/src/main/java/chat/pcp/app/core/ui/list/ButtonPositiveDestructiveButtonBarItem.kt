/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.core.ui.list

import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import com.google.android.material.button.MaterialButton
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.lib.core.utils.epoxy.charsequence.EpoxyCharSequence

/**
 * A generic button list item.
 */
@EpoxyModelClass
abstract class ButtonPositiveDestructiveButtonBarItem : PcpEpoxyModel<ButtonPositiveDestructiveButtonBarItem.Holder>(
        R.layout.item_positive_destrutive_buttons
) {

    @EpoxyAttribute
    var positiveText: EpoxyCharSequence? = null

    @EpoxyAttribute
    var destructiveText: EpoxyCharSequence? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var positiveButtonClickAction: ClickListener? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var destructiveButtonClickAction: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        positiveText?.charSequence?.let { holder.positiveButton.text = it }
        destructiveText?.charSequence?.let { holder.destructiveButton.text = it }

        holder.positiveButton.onClick(positiveButtonClickAction)
        holder.destructiveButton.onClick(destructiveButtonClickAction)
    }

    class Holder : PcpEpoxyHolder() {
        val destructiveButton by bind<MaterialButton>(R.id.destructive_button)
        val positiveButton by bind<MaterialButton>(R.id.positive_button)
    }
}
