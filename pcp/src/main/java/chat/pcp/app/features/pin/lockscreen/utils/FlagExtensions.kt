/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.pin.lockscreen.utils

/**
 * Returns true if the [Int] value contains the provided bit [flag].
 */
internal fun Int.hasFlag(flag: Int) = this and flag == flag
