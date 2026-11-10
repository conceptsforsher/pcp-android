/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.settings.legals

import android.widget.TextView
import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import com.airbnb.epoxy.EpoxyModelWithHolder
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder

/**
 * Displays a single license: name, copyright, and full license text.
 */
@EpoxyModelClass
abstract class LicenseItem : EpoxyModelWithHolder<LicenseItem.Holder>() {

    @EpoxyAttribute lateinit var name: String
    @EpoxyAttribute lateinit var copyright: String
    @EpoxyAttribute lateinit var licenseText: String

    override fun getDefaultLayout(): Int = R.layout.item_license

    override fun bind(holder: Holder) {
        holder.nameView.text = name
        holder.copyrightView.text = copyright
        holder.copyrightView.isVisible = copyright.isNotBlank()
        holder.licenseTextView.text = licenseText
    }

    class Holder : PcpEpoxyHolder() {
        val nameView by bind<TextView>(R.id.licenseName)
        val copyrightView by bind<TextView>(R.id.licenseCopyright)
        val licenseTextView by bind<TextView>(R.id.licenseText)
    }
}
