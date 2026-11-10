/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.platform

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

/**
 * Generic argument with one String. Can be an id (ex: roomId, spaceId, callId, etc.), or anything else
 */
@Parcelize
data class GenericIdArgs(
        val id: String
) : Parcelable
