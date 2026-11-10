/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.popup

import android.app.Activity
import android.view.View
import androidx.annotation.DrawableRes
import chat.pcp.app.R
import chat.pcp.app.core.glide.GlideApp
import chat.pcp.app.databinding.AlerterVerificationLayoutBinding
import chat.pcp.app.features.home.AvatarRenderer
import org.matrix.android.sdk.api.util.MatrixItem

class VerificationPcpAlert(
        uid: String,
        title: String,
        override val description: String,
        @DrawableRes override val iconId: Int?,
        override val priority: Int = PopupAlertManager.DEFAULT_PRIORITY,
        /**
         * Alert are displayed by default, but let this lambda return false to prevent displaying.
         */
        override val shouldBeDisplayedIn: ((Activity) -> Boolean) = { true }
) : DefaultPcpAlert(uid, title, description, iconId, shouldBeDisplayedIn) {
    override val layoutRes = R.layout.alerter_verification_layout

    class ViewBinder(
            private val matrixItem: MatrixItem,
            private val avatarRenderer: AvatarRenderer
    ) : PcpAlert.ViewBinder {

        override fun bind(view: View) {
            val views = AlerterVerificationLayoutBinding.bind(view)
            avatarRenderer.render(matrixItem, views.ivUserAvatar, GlideApp.with(view.context.applicationContext))
        }
    }
}
