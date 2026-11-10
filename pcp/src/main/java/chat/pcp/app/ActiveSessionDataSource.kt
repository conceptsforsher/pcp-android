/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app

import chat.pcp.app.core.utils.BehaviorDataSource
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.util.Optional
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class ActiveSessionDataSource @Inject constructor() : BehaviorDataSource<Optional<Session>>()
