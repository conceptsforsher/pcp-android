/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.userdirectory

import android.widget.ImageView
import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.contacts.MappedContact
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.features.home.AvatarRenderer

@EpoxyModelClass
abstract class ContactItem : PcpEpoxyModel<ContactItem.Holder>(R.layout.item_contact_main) {

    @EpoxyAttribute lateinit var avatarRenderer: AvatarRenderer
    @EpoxyAttribute lateinit var mappedContact: MappedContact

    override fun bind(holder: Holder) {
        super.bind(holder)
        // If name is empty, use userId as name and force it being centered
        holder.nameView.text = mappedContact.displayName
        avatarRenderer.render(mappedContact, holder.avatarImageView)
    }

    class Holder : PcpEpoxyHolder() {
        val nameView by bind<TextView>(R.id.contactDisplayName)
        val avatarImageView by bind<ImageView>(R.id.contactAvatar)
    }
}
