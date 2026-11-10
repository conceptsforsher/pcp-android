/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.alias.detail

import com.airbnb.mvrx.MavericksState

data class RoomAliasBottomSheetState(
        val alias: String,
        val matrixToLink: String? = null,
        val isPublished: Boolean,
        val isMainAlias: Boolean,
        val isLocal: Boolean,
        val canEditCanonicalAlias: Boolean
) : MavericksState {

    constructor(args: RoomAliasBottomSheetArgs) : this(
            alias = args.alias,
            isPublished = args.isPublished,
            isMainAlias = args.isMainAlias,
            isLocal = args.isLocal,
            canEditCanonicalAlias = args.canEditCanonicalAlias
    )
}
