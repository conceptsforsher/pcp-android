/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.login

import chat.pcp.app.core.utils.TemporaryStore
import javax.inject.Inject
import javax.inject.Singleton
import kotlin.time.Duration.Companion.minutes

/**
 * Will store the account password for 3 minutes.
 */
@Singleton
class ReAuthHelper @Inject constructor() : TemporaryStore<String>(3.minutes)
