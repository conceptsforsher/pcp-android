/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy.profiles

import android.widget.ProgressBar
import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R

@EpoxyModelClass
abstract class ProfileMatrixItemWithProgress : BaseProfileMatrixItem<ProfileMatrixItemWithProgress.Holder>(R.layout.item_profile_matrix_item_progress) {

    @EpoxyAttribute var inProgress: Boolean = true

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.progress.isVisible = inProgress
    }

    class Holder : ProfileMatrixItem.Holder() {
        val progress by bind<ProgressBar>(R.id.matrixItemProgress)
    }
}
