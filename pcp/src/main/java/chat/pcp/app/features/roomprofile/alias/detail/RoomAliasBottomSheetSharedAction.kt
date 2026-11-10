/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.alias.detail

import androidx.annotation.DrawableRes
import androidx.annotation.StringRes
import chat.pcp.app.R
import chat.pcp.app.core.platform.PcpSharedAction
import chat.pcp.lib.strings.CommonStrings

sealed class RoomAliasBottomSheetSharedAction(
        @StringRes val titleRes: Int,
        @DrawableRes val iconResId: Int = 0,
        val destructive: Boolean = false
) :
        PcpSharedAction {

    data class ShareAlias(val matrixTo: String) : RoomAliasBottomSheetSharedAction(
            CommonStrings.action_share,
            R.drawable.ic_material_share
    )

    data class PublishAlias(val alias: String) : RoomAliasBottomSheetSharedAction(
            CommonStrings.room_alias_action_publish
    )

    data class UnPublishAlias(val alias: String) : RoomAliasBottomSheetSharedAction(
            CommonStrings.room_alias_action_unpublish
    )

    data class DeleteAlias(val alias: String) : RoomAliasBottomSheetSharedAction(
            CommonStrings.action_delete,
            R.drawable.ic_trash_24,
            true
    )

    data class SetMainAlias(val alias: String) : RoomAliasBottomSheetSharedAction(
            CommonStrings.room_settings_set_main_address
    )

    object UnsetMainAlias : RoomAliasBottomSheetSharedAction(
            CommonStrings.room_settings_unset_main_address
    )
}
