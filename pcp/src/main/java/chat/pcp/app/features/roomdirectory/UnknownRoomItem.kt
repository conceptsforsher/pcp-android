/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomdirectory

import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.features.home.AvatarRenderer
import org.matrix.android.sdk.api.util.MatrixItem

@EpoxyModelClass
abstract class UnknownRoomItem : PcpEpoxyModel<UnknownRoomItem.Holder>(R.layout.item_unknown_room) {

    @EpoxyAttribute
    lateinit var avatarRenderer: AvatarRenderer

    @EpoxyAttribute
    lateinit var matrixItem: MatrixItem

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var globalListener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.rootView.onClick(globalListener)
        avatarRenderer.render(matrixItem, holder.avatarView)
        holder.nameView.text = matrixItem.displayName
    }

    class Holder : PcpEpoxyHolder() {
        val rootView by bind<ViewGroup>(R.id.itemUnknownRoomLayout)
        val avatarView by bind<ImageView>(R.id.itemUnknownRoomAvatar)
        val nameView by bind<TextView>(R.id.itemUnknownRoomName)
    }
}
