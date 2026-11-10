/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.extensions

import android.os.Bundle
import android.os.Parcelable
import com.airbnb.mvrx.Mavericks

fun Parcelable?.toMvRxBundle(): Bundle? {
    return this?.let { Bundle().apply { putParcelable(Mavericks.KEY_ARG, it) } }
}
