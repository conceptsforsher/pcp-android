/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.permissions

import chat.pcp.app.core.resources.StringProvider
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.session.room.powerlevels.Role
import javax.inject.Inject

class RoleFormatter @Inject constructor(
        private val stringProvider: StringProvider
) {
    fun format(role: Role): String {
        return when (role) {
            Role.Admin -> stringProvider.getString(CommonStrings.power_level_admin)
            Role.Moderator -> stringProvider.getString(CommonStrings.power_level_moderator)
            Role.User -> stringProvider.getString(CommonStrings.power_level_default)
            Role.Creator -> stringProvider.getString(CommonStrings.power_level_owner)
            Role.SuperAdmin -> stringProvider.getString(CommonStrings.power_level_owner)
        }
    }
}
