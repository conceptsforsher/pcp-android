/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home.release

import androidx.annotation.DrawableRes
import androidx.annotation.StringRes

class ReleaseCarouselData(
        val items: List<Item>
) {
    data class Item(
            @StringRes val title: Int,
            @StringRes val body: Int,
            @DrawableRes val image: Int,
    )
}
