/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fixtures

import chat.pcp.app.core.resources.BuildMeta

fun aBuildMeta() = BuildMeta(
        isDebug = false,
        applicationId = "im.vector",
        applicationName = "Element",
        lowPrivacyLoggingEnabled = false,
        versionName = "app-version-name",
        gitRevision = "abcdef",
        gitRevisionDate = "01-01-01",
        gitBranchName = "a-branch-name",
        flavorDescription = "Gplay",
        flavorShortDescription = "",
)
