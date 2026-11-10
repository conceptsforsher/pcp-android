/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.analytics

data class AnalyticsConfig(
        val isEnabled: Boolean,
        val postHogHost: String,
        val postHogApiKey: String,
        val policyLink: String,
        val sentryDSN: String,
        val sentryEnvironment: String
)
