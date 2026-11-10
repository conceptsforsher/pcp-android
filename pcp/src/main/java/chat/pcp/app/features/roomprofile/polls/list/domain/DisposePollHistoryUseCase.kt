/*
 * Copyright 2023, 2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.roomprofile.polls.list.domain

import chat.pcp.app.features.roomprofile.polls.list.data.RoomPollRepository
import javax.inject.Inject

class DisposePollHistoryUseCase @Inject constructor(
        private val roomPollRepository: RoomPollRepository,
) {

    fun execute(roomId: String) {
        roomPollRepository.dispose(roomId)
    }
}
