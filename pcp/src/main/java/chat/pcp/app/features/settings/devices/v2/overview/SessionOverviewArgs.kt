/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.overview

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class SessionOverviewArgs(
        val deviceId: String
) : Parcelable
