/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.pin.lockscreen.configuration

import android.os.Parcelable
import chat.pcp.app.features.pin.lockscreen.ui.LockScreenViewModel
import kotlinx.parcelize.Parcelize

/**
 * Mode used by [LockScreenViewModel] to configure the UI and interactions.
 */
@Parcelize
enum class LockScreenMode : Parcelable {
    CREATE,
    VERIFY
}
