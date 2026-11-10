/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fixtures

import chat.pcp.app.features.analytics.plan.UserProperties
import chat.pcp.app.features.analytics.plan.UserProperties.FtueUseCaseSelection

fun aUserProperties(
        ftueUseCase: FtueUseCaseSelection? = FtueUseCaseSelection.Skip
) = UserProperties(
        ftueUseCaseSelection = ftueUseCase
)
