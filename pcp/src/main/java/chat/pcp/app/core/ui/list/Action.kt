/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.ui.list

import chat.pcp.app.core.epoxy.ClickListener

data class Action(
        val title: String,
        val listener: ClickListener
)
