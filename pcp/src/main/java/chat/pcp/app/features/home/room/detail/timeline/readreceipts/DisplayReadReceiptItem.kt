/*
 * Copyright 2023, 2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.readreceipts

import android.widget.ImageView
import android.widget.TextView
import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.features.displayname.getBestName
import chat.pcp.app.features.home.AvatarRenderer
import org.matrix.android.sdk.api.util.MatrixItem

@EpoxyModelClass
abstract class DisplayReadReceiptItem : PcpEpoxyModel<DisplayReadReceiptItem.Holder>(R.layout.item_display_read_receipt) {

    @EpoxyAttribute lateinit var matrixItem: MatrixItem
    @EpoxyAttribute var timestamp: String? = null
    @EpoxyAttribute lateinit var avatarRenderer: AvatarRenderer
    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash) var userClicked: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        avatarRenderer.render(matrixItem, holder.avatarView)
        holder.displayNameView.text = matrixItem.getBestName()
        timestamp?.let {
            holder.timestampView.text = it
            holder.timestampView.isVisible = true
        } ?: run {
            holder.timestampView.isVisible = false
        }
        holder.view.onClick(userClicked)
    }

    class Holder : PcpEpoxyHolder() {
        val avatarView by bind<ImageView>(R.id.readReceiptAvatar)
        val displayNameView by bind<TextView>(R.id.readReceiptName)
        val timestampView by bind<TextView>(R.id.readReceiptDate)
    }
}
