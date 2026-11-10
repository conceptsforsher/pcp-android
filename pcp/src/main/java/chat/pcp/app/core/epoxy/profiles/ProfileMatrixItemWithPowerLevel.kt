/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy.profiles

import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.core.extensions.setTextOrHide

@EpoxyModelClass
abstract class ProfileMatrixItemWithPowerLevel : ProfileMatrixItem() {

    @EpoxyAttribute var ignoredUser: Boolean = false
    @EpoxyAttribute var powerLevelLabel: CharSequence? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.editableView.isVisible = false
        holder.ignoredUserView.isVisible = ignoredUser
        holder.powerLabel.setTextOrHide(powerLevelLabel)
    }
}
