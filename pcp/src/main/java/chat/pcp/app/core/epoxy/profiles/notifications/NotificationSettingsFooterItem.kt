/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy.profiles.notifications

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.extensions.setTextWithColoredPart
import chat.pcp.lib.strings.CommonStrings

@EpoxyModelClass
abstract class NotificationSettingsFooterItem : PcpEpoxyModel<NotificationSettingsFooterItem.Holder>(R.layout.item_notifications_footer) {

    @EpoxyAttribute
    var encrypted: Boolean = false

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var clickListener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        val accountSettingsString = holder.view.context.getString(CommonStrings.room_settings_room_notifications_account_settings)
        val manageNotificationsString = holder.view.context.getString(
                CommonStrings.room_settings_room_notifications_manage_notifications,
                accountSettingsString
        )
        val manageNotificationsBuilder = StringBuilder(manageNotificationsString)
        if (encrypted) {
            val encryptionNotice = holder.view.context.getString(CommonStrings.room_settings_room_notifications_encryption_notice)
            manageNotificationsBuilder.appendLine().append(encryptionNotice)
        }

        holder.textView.setTextWithColoredPart(
                manageNotificationsBuilder.toString(),
                accountSettingsString,
                underline = true
        ) {
            clickListener?.invoke(holder.textView)
        }
    }

    class Holder : PcpEpoxyHolder() {
        val textView by bind<TextView>(R.id.footerText)
    }
}
