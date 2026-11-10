/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.onboarding.ftueauth

import androidx.annotation.DrawableRes
import androidx.annotation.StringRes
import chat.pcp.lib.core.utils.epoxy.charsequence.EpoxyCharSequence

data class SplashCarouselState(
        val items: List<Item>
) {
    data class Item(
            val title: EpoxyCharSequence,
            @StringRes val body: Int,
            @DrawableRes val image: Int,
            @DrawableRes val pageBackground: Int
    )
}
