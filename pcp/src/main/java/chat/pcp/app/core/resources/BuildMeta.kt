/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.resources

data class BuildMeta(
        val isDebug: Boolean,
        val applicationId: String,
        val applicationName: String,
        val lowPrivacyLoggingEnabled: Boolean,
        val versionName: String,
        val gitRevision: String,
        val gitRevisionDate: String,
        val gitBranchName: String,
        val flavorDescription: String,
        val flavorShortDescription: String,
)
