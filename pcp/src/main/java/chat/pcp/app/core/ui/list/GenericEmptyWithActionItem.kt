/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.core.ui.list

import android.content.res.ColorStateList
import android.view.View
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import androidx.annotation.ColorInt
import androidx.annotation.DrawableRes
import androidx.core.view.isVisible
import androidx.core.widget.ImageViewCompat
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.extensions.setTextOrHide

/**
 * A generic list item to display when there is no results, with an optional CTA.
 */
@EpoxyModelClass
abstract class GenericEmptyWithActionItem : PcpEpoxyModel<GenericEmptyWithActionItem.Holder>(R.layout.item_generic_empty_state) {

    @EpoxyAttribute
    var title: String? = null

    @EpoxyAttribute
    var description: String? = null

    @EpoxyAttribute
    @DrawableRes
    var iconRes: Int = -1

    @EpoxyAttribute
    @ColorInt
    var iconTint: Int? = null

    @EpoxyAttribute
    var buttonAction: Action? = null

    override fun bind(holder: Holder) {
        super.bind(holder)

        holder.titleText.setTextOrHide(title)
        holder.descriptionText.setTextOrHide(description)

        if (iconRes != -1) {
            holder.imageView.setImageResource(iconRes)
            holder.imageView.isVisible = true
            if (iconTint != null) {
                ImageViewCompat.setImageTintList(holder.imageView, ColorStateList.valueOf(iconTint!!))
            } else {
                ImageViewCompat.setImageTintList(holder.imageView, null)
            }
        } else {
            holder.imageView.isVisible = false
        }

        holder.actionButton.setTextOrHide(buttonAction?.title)
        holder.actionButton.onClick(buttonAction?.listener)
    }

    class Holder : PcpEpoxyHolder() {
        val root by bind<View>(R.id.item_generic_root)
        val titleText by bind<TextView>(R.id.emptyItemTitleView)
        val descriptionText by bind<TextView>(R.id.emptyItemMessageView)
        val imageView by bind<ImageView>(R.id.emptyItemImageView)
        val actionButton by bind<Button>(R.id.emptyItemButton)
    }
}
