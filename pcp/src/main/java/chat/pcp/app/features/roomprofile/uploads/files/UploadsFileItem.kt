/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.uploads.files

import android.view.View
import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.extensions.setTextOrHide

@EpoxyModelClass
abstract class UploadsFileItem : PcpEpoxyModel<UploadsFileItem.Holder>(R.layout.item_uploads_file) {

    @EpoxyAttribute var title: String? = null
    @EpoxyAttribute var subtitle: String? = null

    @EpoxyAttribute var listener: Listener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.view.setOnClickListener { listener?.onItemClicked() }
        holder.titleView.text = title
        holder.subtitleView.setTextOrHide(subtitle)
        holder.downloadView.setOnClickListener { listener?.onDownloadClicked() }
        holder.shareView.setOnClickListener { listener?.onShareClicked() }
    }

    class Holder : PcpEpoxyHolder() {
        val titleView by bind<TextView>(R.id.uploadsFileTitle)
        val subtitleView by bind<TextView>(R.id.uploadsFileSubtitle)
        val downloadView by bind<View>(R.id.uploadsFileActionDownload)
        val shareView by bind<View>(R.id.uploadsFileActionShare)
    }

    interface Listener {
        fun onItemClicked()
        fun onDownloadClicked()
        fun onShareClicked()
    }
}
