/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.details

import chat.pcp.app.core.session.clientinfo.MatrixClientInfoContent
import org.matrix.android.sdk.api.extensions.orFalse
import javax.inject.Inject

class CheckIfSectionApplicationIsVisibleUseCase @Inject constructor() {

    fun execute(matrixClientInfoContent: MatrixClientInfoContent?): Boolean {
        return matrixClientInfoContent?.name?.isNotEmpty().orFalse() ||
                matrixClientInfoContent?.version?.isNotEmpty().orFalse() ||
                matrixClientInfoContent?.url?.isNotEmpty().orFalse()
    }
}
