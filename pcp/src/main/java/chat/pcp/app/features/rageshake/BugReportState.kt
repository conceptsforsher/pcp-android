/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.rageshake

import com.airbnb.mvrx.MavericksState

data class BugReportState(
        val serverVersion: String = ""
) : MavericksState
