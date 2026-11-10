/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.discovery

import android.widget.Button
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick

@EpoxyModelClass
abstract class SettingsContinueCancelItem : PcpEpoxyModel<SettingsContinueCancelItem.Holder>(R.layout.item_settings_continue_cancel) {

    @EpoxyAttribute
    var continueText: String? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var continueOnClick: ClickListener? = null

    @EpoxyAttribute
    var canContinue: Boolean = true

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var cancelOnClick: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)

        holder.cancelButton.onClick(cancelOnClick)

        continueText?.let { holder.continueButton.text = it }
        holder.continueButton.onClick(continueOnClick)
        holder.continueButton.isEnabled = canContinue
    }

    class Holder : PcpEpoxyHolder() {
        val cancelButton by bind<Button>(R.id.settings_item_cancel_button)
        val continueButton by bind<Button>(R.id.settings_item_continue_button)
    }
}
