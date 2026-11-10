/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.lib.attachmentviewer

import android.view.View
import chat.pcp.lib.attachmentviewer.databinding.ItemAnimatedImageAttachmentBinding

class AnimatedImageViewHolder constructor(itemView: View) :
        BaseViewHolder(itemView) {

    val views = ItemAnimatedImageAttachmentBinding.bind(itemView)

    internal val target = DefaultImageLoaderTarget(this, views.imageView)

    override fun onRecycled() {
        super.onRecycled()
        views.imageView.setImageDrawable(null)
    }
}
