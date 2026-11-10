/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomdirectory.picker

import android.view.View
import android.widget.TextView
import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.extensions.setTextOrHide

@EpoxyModelClass
abstract class RoomDirectoryServerItem : PcpEpoxyModel<RoomDirectoryServerItem.Holder>(R.layout.item_room_directory_server) {

    @EpoxyAttribute
    var serverName: String? = null

    @EpoxyAttribute
    var serverDescription: String? = null

    @EpoxyAttribute
    var canRemove: Boolean = false

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var removeListener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.nameView.text = serverName
        holder.descriptionView.setTextOrHide(serverDescription)
        holder.deleteView.isVisible = canRemove
        holder.deleteView.onClick(removeListener)
    }

    class Holder : PcpEpoxyHolder() {
        val nameView by bind<TextView>(R.id.itemRoomDirectoryServerName)
        val descriptionView by bind<TextView>(R.id.itemRoomDirectoryServerDescription)
        val deleteView by bind<View>(R.id.itemRoomDirectoryServerRemove)
    }
}
