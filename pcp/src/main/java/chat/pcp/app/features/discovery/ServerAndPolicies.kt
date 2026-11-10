/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.discovery

data class ServerAndPolicies(
        val serverUrl: String,
        val policies: List<ServerPolicy>
)

data class ServerPolicy(
        val name: String,
        val url: String
)
