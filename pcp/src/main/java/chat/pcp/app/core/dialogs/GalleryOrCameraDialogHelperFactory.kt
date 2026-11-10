/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.dialogs

import androidx.fragment.app.Fragment
import chat.pcp.app.core.resources.ColorProvider
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.lib.core.utils.timer.Clock
import javax.inject.Inject

class GalleryOrCameraDialogHelperFactory @Inject constructor(
        private val colorProvider: ColorProvider,
        private val clock: Clock,
        private val pcpPreferences: PcpBasePreferences,
) {
    fun create(fragment: Fragment, skipCrop: Boolean = false): GalleryOrCameraDialogHelper {
        return GalleryOrCameraDialogHelper(fragment, colorProvider, clock, pcpPreferences, skipCrop)
    }
}
