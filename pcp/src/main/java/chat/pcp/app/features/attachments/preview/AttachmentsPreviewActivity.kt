/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.attachments.preview

import android.content.Context
import android.content.Intent
import android.view.View
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.addFragment
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.databinding.ActivitySimpleBinding
import chat.pcp.app.features.themes.ActivityOtherThemes
import chat.pcp.lib.core.utils.compat.getParcelableArrayListExtraCompat
import chat.pcp.lib.core.utils.compat.getParcelableCompat
import org.matrix.android.sdk.api.session.content.ContentAttachmentData

@AndroidEntryPoint
class AttachmentsPreviewActivity : PcpActivity<ActivitySimpleBinding>() {

    companion object {
        private const val EXTRA_FRAGMENT_ARGS = "EXTRA_FRAGMENT_ARGS"
        private const val ATTACHMENTS_PREVIEW_RESULT = "ATTACHMENTS_PREVIEW_RESULT"
        private const val KEEP_ORIGINAL_IMAGES_SIZE = "KEEP_ORIGINAL_IMAGES_SIZE"

        fun newIntent(context: Context, args: AttachmentsPreviewArgs): Intent {
            return Intent(context, AttachmentsPreviewActivity::class.java).apply {
                putExtra(EXTRA_FRAGMENT_ARGS, args)
            }
        }

        fun getOutput(intent: Intent): List<ContentAttachmentData> {
            return intent.getParcelableArrayListExtraCompat<ContentAttachmentData>(ATTACHMENTS_PREVIEW_RESULT).orEmpty()
        }

        fun getKeepOriginalSize(intent: Intent): Boolean {
            return intent.getBooleanExtra(KEEP_ORIGINAL_IMAGES_SIZE, false)
        }
    }

    override fun getOtherThemes() = ActivityOtherThemes.AttachmentsPreview

    override fun getBinding() = ActivitySimpleBinding.inflate(layoutInflater)

    override fun getCoordinatorLayout() = views.coordinatorLayout

    override val rootView: View
        get() = views.coordinatorLayout

    override fun initUiAndData() {
        if (isFirstCreation()) {
            val fragmentArgs: AttachmentsPreviewArgs = intent?.extras?.getParcelableCompat(EXTRA_FRAGMENT_ARGS) ?: return
            addFragment(views.simpleFragmentContainer, AttachmentsPreviewFragment::class.java, fragmentArgs)
        }
    }

    fun setResultAndFinish(data: List<ContentAttachmentData>, keepOriginalImageSize: Boolean) {
        val resultIntent = Intent().apply {
            putParcelableArrayListExtra(ATTACHMENTS_PREVIEW_RESULT, ArrayList(data))
            putExtra(KEEP_ORIGINAL_IMAGES_SIZE, keepOriginalImageSize)
        }
        setResult(RESULT_OK, resultIntent)
        finish()
    }
}
