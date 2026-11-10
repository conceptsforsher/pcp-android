/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.manage

import android.widget.ImageView
import android.widget.TextView
import androidx.core.content.ContextCompat
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
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.util.MatrixItem

@EpoxyModelClass
abstract class RoomManageSelectionItem : PcpEpoxyModel<RoomManageSelectionItem.Holder>(R.layout.item_room_to_manage_in_space) {

    @EpoxyAttribute lateinit var avatarRenderer: AvatarRenderer
    @EpoxyAttribute lateinit var matrixItem: MatrixItem
    @EpoxyAttribute var selected: Boolean = false
    @EpoxyAttribute var suggested: Boolean = false
    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash) var itemClickListener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        avatarRenderer.render(matrixItem, holder.avatarImageView)

        holder.titleText.text = matrixItem.getBestName()

        if (selected) {
            holder.checkboxImage.setImageDrawable(ContextCompat.getDrawable(holder.view.context, R.drawable.ic_checkbox_on))
            holder.checkboxImage.contentDescription = holder.view.context.getString(CommonStrings.a11y_checked)
        } else {
            holder.checkboxImage.setImageDrawable(ContextCompat.getDrawable(holder.view.context, R.drawable.ic_checkbox_off))
            holder.checkboxImage.contentDescription = holder.view.context.getString(CommonStrings.a11y_unchecked)
        }

        holder.suggestedText.isVisible = suggested

        holder.view.onClick(itemClickListener)
    }

    class Holder : PcpEpoxyHolder() {
        val avatarImageView by bind<ImageView>(R.id.itemAddRoomRoomAvatar)
        val titleText by bind<TextView>(R.id.itemAddRoomRoomNameText)
        val suggestedText by bind<TextView>(R.id.itemManageRoomSuggested)
        val checkboxImage by bind<ImageView>(R.id.itemAddRoomRoomCheckBox)
    }
}
