/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.crypto.verification.epoxy

import android.content.res.ColorStateList
import android.widget.ImageView
import android.widget.TextView
import androidx.annotation.DrawableRes
import androidx.core.view.isVisible
import androidx.core.widget.ImageViewCompat
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.extensions.setTextOrHide

/**
 * A action for bottom sheet.
 */
@EpoxyModelClass
abstract class BottomSheetVerificationActionItem : PcpEpoxyModel<BottomSheetVerificationActionItem.Holder>(R.layout.item_verification_action) {

    @EpoxyAttribute
    @DrawableRes
    var iconRes: Int = -1

    @EpoxyAttribute
    var title: String = ""

    @EpoxyAttribute
    var subTitle: String? = null

    @EpoxyAttribute
    var titleColor: Int = 0

    @EpoxyAttribute
    var iconColor: Int = -1

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    lateinit var listener: ClickListener

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.view.onClick(listener)
        holder.title.text = title
        holder.title.setTextColor(titleColor)

        holder.subTitle.setTextOrHide(subTitle)

        if (iconRes != -1) {
            holder.icon.isVisible = true
            holder.icon.setImageResource(iconRes)
            if (iconColor != -1) {
                ImageViewCompat.setImageTintList(holder.icon, ColorStateList.valueOf(iconColor))
            }
        } else {
            holder.icon.isVisible = false
        }
    }

    class Holder : PcpEpoxyHolder() {
        val title by bind<TextView>(R.id.itemVerificationActionTitle)
        val subTitle by bind<TextView>(R.id.itemVerificationActionSubTitle)
        val icon by bind<ImageView>(R.id.itemVerificationActionIcon)
    }
}
