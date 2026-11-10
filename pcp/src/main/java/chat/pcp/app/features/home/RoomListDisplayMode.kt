/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import androidx.annotation.StringRes
import chat.pcp.lib.strings.CommonStrings

enum class RoomListDisplayMode(@StringRes val titleRes: Int) {
    NOTIFICATIONS(CommonStrings.bottom_action_notification),
    PEOPLE(CommonStrings.bottom_action_people_x),
    ROOMS(CommonStrings.bottom_action_rooms),
    FILTERED(/* Not used */ 0)
}
