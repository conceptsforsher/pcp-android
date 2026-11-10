/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.discovery

import android.widget.ImageView
import androidx.annotation.DrawableRes
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel

@EpoxyModelClass
abstract class SettingsCenteredImageItem : PcpEpoxyModel<SettingsCenteredImageItem.Holder>(R.layout.item_settings_centered_image) {

    @EpoxyAttribute
    @DrawableRes
    var drawableRes: Int = 0

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.image.setImageResource(drawableRes)
    }

    class Holder : PcpEpoxyHolder() {
        val image by bind<ImageView>(R.id.itemSettingsImage)
    }
}
