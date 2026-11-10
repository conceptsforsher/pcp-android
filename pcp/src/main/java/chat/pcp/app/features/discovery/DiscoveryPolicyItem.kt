/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.discovery

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.extensions.setTextOrHide

@EpoxyModelClass
abstract class DiscoveryPolicyItem : PcpEpoxyModel<DiscoveryPolicyItem.Holder>(R.layout.item_discovery_policy) {

    @EpoxyAttribute
    var name: String? = null

    @EpoxyAttribute
    var url: String? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var clickListener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.title.text = name
        holder.url.setTextOrHide(url)
        holder.view.onClick(clickListener)
    }

    class Holder : PcpEpoxyHolder() {
        val title by bind<TextView>(R.id.discovery_policy_name)
        val url by bind<TextView>(R.id.discovery_policy_url)
    }
}
