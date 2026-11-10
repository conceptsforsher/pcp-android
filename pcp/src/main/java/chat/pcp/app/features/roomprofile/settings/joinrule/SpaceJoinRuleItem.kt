/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.settings.joinrule

import android.widget.Button
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
import chat.pcp.app.core.extensions.setAttributeTintedImageResource
import chat.pcp.app.core.utils.DebouncedClickListener
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.util.MatrixItem

@EpoxyModelClass
abstract class SpaceJoinRuleItem : PcpEpoxyModel<SpaceJoinRuleItem.Holder>(R.layout.item_bottom_sheet_joinrule_restricted) {

    @EpoxyAttribute
    var selected: Boolean = false

    @EpoxyAttribute
    var needUpgrade: Boolean = false

    @EpoxyAttribute
    lateinit var avatarRenderer: AvatarRenderer

    @EpoxyAttribute
    var restrictedList: List<MatrixItem> = emptyList()

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    lateinit var listener: ClickListener

    override fun bind(holder: Holder) {
        super.bind(holder)

        holder.view.onClick(listener)
        holder.upgradeRequiredButton.setOnClickListener(DebouncedClickListener(listener))

        if (selected) {
            holder.radioImage.setAttributeTintedImageResource(R.drawable.ic_radio_on, com.google.android.material.R.attr.colorPrimary)
            holder.radioImage.contentDescription = holder.view.context.getString(CommonStrings.a11y_checked)
        } else {
            holder.radioImage.setImageDrawable(ContextCompat.getDrawable(holder.view.context, R.drawable.ic_radio_off))
            holder.radioImage.contentDescription = holder.view.context.getString(CommonStrings.a11y_unchecked)
        }

        holder.upgradeRequiredButton.isVisible = needUpgrade
        holder.helperText.isVisible = selected

        val items = listOf(holder.space1, holder.space2, holder.space3, holder.space4, holder.space5)
        holder.spaceMore.isVisible = false
        items.onEach { it.isVisible = false }
        if (!needUpgrade) {
            if (restrictedList.isEmpty()) {
                holder.listTitle.isVisible = false
            } else {
                holder.listTitle.isVisible = true
                restrictedList.forEachIndexed { index, matrixItem ->
                    if (index < items.size) {
                        items[index].isVisible = true
                        avatarRenderer.render(matrixItem, items[index])
                    } else if (index == items.size) {
                        holder.spaceMore.isVisible = true
                    }
                }
            }
        } else {
            holder.listTitle.isVisible = false
            holder.helperText.isVisible = false
        }
    }

    class Holder : PcpEpoxyHolder() {
        val radioImage by bind<ImageView>(R.id.radioIcon)
        val actionTitle by bind<TextView>(R.id.actionTitle)
        val actionDescription by bind<TextView>(R.id.actionDescription)
        val upgradeRequiredButton by bind<Button>(R.id.upgradeRequiredButton)
        val listTitle by bind<TextView>(R.id.listTitle)
        val space1 by bind<ImageView>(R.id.rest1)
        val space2 by bind<ImageView>(R.id.rest2)
        val space3 by bind<ImageView>(R.id.rest3)
        val space4 by bind<ImageView>(R.id.rest4)
        val space5 by bind<ImageView>(R.id.rest5)
        val spaceMore by bind<ImageView>(R.id.rest6)
        val helperText by bind<TextView>(R.id.helperText)
    }
}
