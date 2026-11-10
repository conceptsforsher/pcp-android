/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.dialogs

import android.app.Activity
import androidx.core.view.isVisible
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import chat.pcp.app.R
import chat.pcp.app.databinding.DialogPhotoOrVideoBinding
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.lib.strings.CommonStrings

class PhotoOrVideoDialog(
        private val activity: Activity,
        private val pcpPreferences: PcpBasePreferences
) {

    interface PhotoOrVideoDialogListener {
        fun takePhoto()
        fun takeVideo()
    }

    interface PhotoOrVideoDialogSettingsListener {
        fun onUpdated()
    }

    fun show(listener: PhotoOrVideoDialogListener) {
        when (pcpPreferences.getTakePhotoVideoMode()) {
            PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_PHOTO -> listener.takePhoto()
            PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_VIDEO -> listener.takeVideo()
            /* PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_ALWAYS_ASK */
            else -> {
                val dialogLayout = activity.layoutInflater.inflate(R.layout.dialog_photo_or_video, null)
                val views = DialogPhotoOrVideoBinding.bind(dialogLayout)

                // Show option to set as default in this case
                views.dialogPhotoOrVideoAsDefault.isVisible = true
                // Always default to photo
                views.dialogPhotoOrVideoPhoto.isChecked = true

                MaterialAlertDialogBuilder(activity)
                        .setTitle(CommonStrings.option_take_photo_video)
                        .setView(dialogLayout)
                        .setPositiveButton(CommonStrings._continue) { _, _ ->
                            submit(views, pcpPreferences, listener)
                        }
                        .setNegativeButton(CommonStrings.action_cancel, null)
                        .show()
            }
        }
    }

    private fun submit(
            views: DialogPhotoOrVideoBinding,
            pcpPreferences: PcpBasePreferences,
            listener: PhotoOrVideoDialogListener
    ) {
        val mode = if (views.dialogPhotoOrVideoPhoto.isChecked) {
            PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_PHOTO
        } else {
            PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_VIDEO
        }

        if (views.dialogPhotoOrVideoAsDefault.isChecked) {
            pcpPreferences.setTakePhotoVideoMode(mode)
        }

        when (mode) {
            PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_PHOTO -> listener.takePhoto()
            PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_VIDEO -> listener.takeVideo()
        }
    }

    fun showForSettings(listener: PhotoOrVideoDialogSettingsListener) {
        val currentMode = pcpPreferences.getTakePhotoVideoMode()

        val dialogLayout = activity.layoutInflater.inflate(R.layout.dialog_photo_or_video, null)
        val views = DialogPhotoOrVideoBinding.bind(dialogLayout)

        // Show option for always ask in this case
        views.dialogPhotoOrVideoAlwaysAsk.isVisible = true
        // Always default to photo
        views.dialogPhotoOrVideoPhoto.isChecked = currentMode == PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_PHOTO
        views.dialogPhotoOrVideoVideo.isChecked = currentMode == PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_VIDEO
        views.dialogPhotoOrVideoAlwaysAsk.isChecked = currentMode == PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_ALWAYS_ASK

        MaterialAlertDialogBuilder(activity)
                .setTitle(CommonStrings.option_take_photo_video)
                .setView(dialogLayout)
                .setPositiveButton(CommonStrings.action_save) { _, _ ->
                    submitSettings(views)
                    listener.onUpdated()
                }
                .setNegativeButton(CommonStrings.action_cancel, null)
                .show()
    }

    private fun submitSettings(views: DialogPhotoOrVideoBinding) {
        pcpPreferences.setTakePhotoVideoMode(
                when {
                    views.dialogPhotoOrVideoPhoto.isChecked -> PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_PHOTO
                    views.dialogPhotoOrVideoVideo.isChecked -> PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_VIDEO
                    else -> PcpBasePreferences.TAKE_PHOTO_VIDEO_MODE_ALWAYS_ASK
                }
        )
    }
}
