/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.platform

import android.os.Bundle

interface Restorable {

    fun onSaveInstanceState(outState: Bundle)

    fun onRestoreInstanceState(savedInstanceState: Bundle?)
}
