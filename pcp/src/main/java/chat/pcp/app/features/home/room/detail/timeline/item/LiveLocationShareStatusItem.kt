/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline.item

import android.widget.ImageView
import chat.pcp.app.features.home.room.detail.timeline.style.TimelineMessageLayout

interface LiveLocationShareStatusItem {
    fun bindMap(
            mapImageView: ImageView,
            mapWidth: Int,
            mapHeight: Int,
            messageLayout: TimelineMessageLayout
    )

    fun bindBottomBanner(bannerImageView: ImageView, messageLayout: TimelineMessageLayout)
}
