/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home.header

import androidx.annotation.StringRes
import chat.pcp.lib.strings.CommonStrings

enum class HomeRoomFilter(@StringRes val titleRes: Int) {
    ALL(CommonStrings.room_list_filter_all),
    UNREADS(CommonStrings.room_list_filter_unreads),
    FAVOURITES(CommonStrings.room_list_filter_favourites),
    PEOPlE(CommonStrings.room_list_filter_people),
}
