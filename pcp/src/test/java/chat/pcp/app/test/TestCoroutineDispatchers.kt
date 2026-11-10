/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test

import kotlinx.coroutines.test.UnconfinedTestDispatcher
import org.matrix.android.sdk.api.MatrixCoroutineDispatchers

internal val testDispatcher = UnconfinedTestDispatcher()

internal val testCoroutineDispatchers = MatrixCoroutineDispatchers(
        io = testDispatcher,
        computation = testDispatcher,
        main = testDispatcher,
        crypto = testDispatcher,
        dmVerif = testDispatcher
)
