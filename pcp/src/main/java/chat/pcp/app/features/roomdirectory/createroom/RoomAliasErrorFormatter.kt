/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomdirectory.createroom

import chat.pcp.app.core.resources.StringProvider
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.session.room.alias.RoomAliasError
import javax.inject.Inject

class RoomAliasErrorFormatter @Inject constructor(
        private val stringProvider: StringProvider
) {
    fun format(roomAliasError: RoomAliasError?): String? {
        return when (roomAliasError) {
            is RoomAliasError.AliasIsBlank -> CommonStrings.create_room_alias_empty
            is RoomAliasError.AliasNotAvailable -> CommonStrings.create_room_alias_already_in_use
            is RoomAliasError.AliasInvalid -> CommonStrings.create_room_alias_invalid
            else -> null
        }
                ?.let { stringProvider.getString(it) }
    }
}
