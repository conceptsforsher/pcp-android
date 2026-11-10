/*
 * Copyright 2023, 2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.mdm

import android.content.Context
import timber.log.Timber

enum class MdmData(val key: String) {
    DefaultHomeserverUrl(key = "chat.pcp.app.serverConfigDefaultHomeserverUrlString"),
    DefaultPushGatewayUrl(key = "chat.pcp.app.serverConfigSygnalAPIUrlString"),
    PermalinkBaseUrl(key = "chat.pcp.app.clientPermalinkBaseUrl"),
}

interface MdmService {
    fun registerListener(context: Context, onChangedListener: () -> Unit)
    fun unregisterListener(context: Context)
    fun getData(mdmData: MdmData): String?
    fun getData(mdmData: MdmData, defaultValue: String): String {
        return getData(mdmData)
                ?.also {
                    Timber.w("Using MDM data for ${mdmData.name}: $it")
                }
                ?: defaultValue
    }
}
