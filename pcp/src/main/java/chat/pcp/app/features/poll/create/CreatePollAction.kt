/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.poll.create

import chat.pcp.app.core.platform.PcpViewModelAction
import org.matrix.android.sdk.api.session.room.model.message.PollType

sealed class CreatePollAction : PcpViewModelAction {
    data class OnQuestionChanged(val question: String) : CreatePollAction()
    data class OnOptionChanged(val index: Int, val option: String) : CreatePollAction()
    data class OnDeleteOption(val index: Int) : CreatePollAction()
    data class OnPollTypeChanged(val pollType: PollType) : CreatePollAction()
    object OnAddOption : CreatePollAction()
    object OnCreatePoll : CreatePollAction()
}
