/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.displayname

import org.matrix.android.sdk.api.provider.MatrixItemDisplayNameFallbackProvider
import org.matrix.android.sdk.api.util.MatrixItem

// Used to provide the fallback to the MatrixSDK, in the MatrixConfiguration
object PcpDisplayNameFallback : MatrixItemDisplayNameFallbackProvider {
    override fun getDefaultName(matrixItem: MatrixItem): String {
        // Can customize something from the id if necessary here
        return matrixItem.id
    }
}
