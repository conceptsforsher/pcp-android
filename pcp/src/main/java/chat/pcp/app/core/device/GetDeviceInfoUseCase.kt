/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.device

import chat.pcp.app.core.di.ActiveSessionHolder
import org.matrix.android.sdk.api.session.crypto.model.CryptoDeviceInfo
import javax.inject.Inject

interface GetDeviceInfoUseCase {

    suspend fun execute(): CryptoDeviceInfo
}

class DefaultGetDeviceInfoUseCase @Inject constructor(
        private val activeSessionHolder: ActiveSessionHolder
) : GetDeviceInfoUseCase {

    override suspend fun execute(): CryptoDeviceInfo {
        return activeSessionHolder.getActiveSession().cryptoService().getMyCryptoDevice()
    }
}
