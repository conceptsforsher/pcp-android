/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.crypto.keysbackup.settings

import android.widget.Button
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
abstract class KeysBackupSettingFooterItem : PcpEpoxyModel<KeysBackupSettingFooterItem.Holder>(R.layout.item_keys_backup_settings_button_footer) {

    @EpoxyAttribute
    var textButton1: String? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var clickOnButton1: ClickListener? = null

    @EpoxyAttribute
    var textButton2: String? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var clickOnButton2: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.button1.setTextOrHide(textButton1)
        holder.button1.onClick(clickOnButton1)

        holder.button2.setTextOrHide(textButton2)
        holder.button2.onClick(clickOnButton2)
    }

    class Holder : PcpEpoxyHolder() {
        val button1 by bind<Button>(R.id.keys_backup_settings_footer_button1)
        val button2 by bind<TextView>(R.id.keys_backup_settings_footer_button2)
    }
}
