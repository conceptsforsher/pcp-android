/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home.release

import android.widget.ImageView
import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel

@EpoxyModelClass
abstract class ReleaseCarouselItem : PcpEpoxyModel<ReleaseCarouselItem.Holder>(R.layout.item_release_carousel) {

    @EpoxyAttribute
    lateinit var item: ReleaseCarouselData.Item

    override fun bind(holder: Holder) {
        super.bind(holder)

        holder.image.setImageResource(item.image)
        holder.title.setText(item.title)
        holder.body.setText(item.body)
    }

    class Holder : PcpEpoxyHolder() {
        val image by bind<ImageView>(R.id.carousel_item_image)
        val title by bind<TextView>(R.id.carousel_item_title)
        val body by bind<TextView>(R.id.carousel_item_body)
    }
}
