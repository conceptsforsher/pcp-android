/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.session.clientinfo

import chat.pcp.app.core.di.ActiveSessionHolder
import timber.log.Timber
import javax.inject.Inject

/**
 * This use case delete the account data event containing extended client info.
 */
class DeleteMatrixClientInfoUseCase @Inject constructor(
        private val activeSessionHolder: ActiveSessionHolder,
        private val setMatrixClientInfoUseCase: SetMatrixClientInfoUseCase,
) {

    suspend fun execute(): Result<Unit> = runCatching {
        Timber.d("deleting recorded client info")
        val session = activeSessionHolder.getActiveSession()
        val emptyClientInfo = MatrixClientInfoContent(
                name = "",
                version = "",
                url = "",
        )
        return setMatrixClientInfoUseCase.execute(session, emptyClientInfo)
    }
}
