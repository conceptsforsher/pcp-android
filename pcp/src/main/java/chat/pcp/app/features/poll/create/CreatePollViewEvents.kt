/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.poll.create

import chat.pcp.app.core.platform.PcpViewEvents

sealed class CreatePollViewEvents : PcpViewEvents {
    object Success : CreatePollViewEvents()
    object EmptyQuestionError : CreatePollViewEvents()
    data class NotEnoughOptionsError(val requiredOptionsCount: Int) : CreatePollViewEvents()
}
