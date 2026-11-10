/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.grouplist

import android.content.res.ColorStateList
import android.view.View
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
import chat.pcp.app.core.platform.CheckableConstraintLayout
import chat.pcp.app.features.home.room.list.UnreadCounterBadgeView
import chat.pcp.app.features.themes.ThemeUtils
import chat.pcp.lib.strings.CommonStrings

@EpoxyModelClass
abstract class HomeSpaceSummaryItem : PcpEpoxyModel<HomeSpaceSummaryItem.Holder>(R.layout.item_space) {

    @EpoxyAttribute var text: String = ""
    @EpoxyAttribute var selected: Boolean = false
    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash) var listener: ClickListener? = null
    @EpoxyAttribute var countState: UnreadCounterBadgeView.State = UnreadCounterBadgeView.State.Count(0, false)
    @EpoxyAttribute var showSeparator: Boolean = false

    override fun getViewType(): Int {
        // mm.. it's reusing the same layout for basic space item
        return R.id.space_item_home
    }

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.rootView.onClick(listener)
        holder.groupNameView.text = holder.view.context.getString(CommonStrings.group_details_home)
        holder.rootView.isChecked = selected
        holder.rootView.context.resources
        holder.avatarImageView.background = ContextCompat.getDrawable(holder.view.context, R.drawable.space_home_background)
        holder.avatarImageView.setImageResource(R.drawable.ic_space_home)
        holder.avatarImageView.imageTintList = ColorStateList.valueOf(
                ThemeUtils.getColor(holder.view.context, chat.pcp.lib.ui.styles.R.attr.vctr_content_primary)
        )
        holder.avatarImageView.scaleType = ImageView.ScaleType.CENTER_INSIDE
        holder.leaveView.isVisible = false

        holder.counterBadgeView.render(countState)
        holder.bottomSeparator.isVisible = showSeparator
    }

    class Holder : PcpEpoxyHolder() {
        val avatarImageView by bind<ImageView>(R.id.groupAvatarImageView)
        val groupNameView by bind<TextView>(R.id.groupNameView)
        val rootView by bind<CheckableConstraintLayout>(R.id.itemGroupLayout)
        val leaveView by bind<ImageView>(R.id.groupTmpLeave)
        val counterBadgeView by bind<UnreadCounterBadgeView>(R.id.groupCounterBadge)
        val bottomSeparator by bind<View>(R.id.groupBottomSeparator)
    }
}
