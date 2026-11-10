/*
 * Copyright 2021-2025 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.legals

import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.features.discovery.ServerPolicy
import chat.pcp.app.features.settings.PcpSettingsUrls
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

class ElementLegals @Inject constructor(
        private val stringProvider: StringProvider
) {
    fun getData(): List<ServerPolicy> {
        return listOf(
                ServerPolicy(stringProvider.getString(CommonStrings.settings_copyright), PcpSettingsUrls.COPYRIGHT),
        )
    }
}
