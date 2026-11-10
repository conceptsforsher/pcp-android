/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.platform

/**
 * Model to display a Waiting View.
 */
data class WaitingViewData(
        val message: String,
        val progress: Int? = null,
        val progressTotal: Int? = null,
        val isIndeterminate: Boolean = false
)
