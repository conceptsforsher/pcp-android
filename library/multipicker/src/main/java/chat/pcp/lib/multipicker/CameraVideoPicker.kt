/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.lib.multipicker

import android.content.Context
import android.content.Intent
import android.net.Uri
import android.provider.MediaStore
import androidx.activity.result.ActivityResultLauncher
import androidx.core.content.FileProvider
import chat.pcp.lib.multipicker.entity.MultiPickerVideoType
import chat.pcp.lib.multipicker.utils.MediaType
import chat.pcp.lib.multipicker.utils.createTemporaryMediaFile
import chat.pcp.lib.multipicker.utils.toMultiPickerVideoType

/**
 * Implementation of taking a video with Camera.
 */
class CameraVideoPicker {

    /**
     * Start camera by using a ActivityResultLauncher.
     * @return Uri of taken photo or null if the operation is cancelled.
     */
    fun startWithExpectingFile(context: Context, activityResultLauncher: ActivityResultLauncher<Intent>): Uri {
        val videoUri = createVideoUri(context)
        val intent = createIntent().apply {
            putExtra(MediaStore.EXTRA_OUTPUT, videoUri)
        }
        activityResultLauncher.launch(intent)
        return videoUri
    }

    /**
     * Call this function from onActivityResult(int, int, Intent).
     * @return Taken photo or null if request code is wrong
     * or result code is not Activity.RESULT_OK
     * or user cancelled the operation.
     */
    fun getTakenVideo(context: Context, videoUri: Uri): MultiPickerVideoType? {
        return videoUri.toMultiPickerVideoType(context)
    }

    private fun createIntent(): Intent {
        return Intent(MediaStore.ACTION_VIDEO_CAPTURE)
    }

    companion object {
        fun createVideoUri(context: Context): Uri {
            val file = createTemporaryMediaFile(context, MediaType.VIDEO)
            val authority = context.packageName + ".multipicker.fileprovider"
            return FileProvider.getUriForFile(context, authority, file)
        }
    }
}
