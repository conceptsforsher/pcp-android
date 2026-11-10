/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.ui.views

import android.content.Context
import android.util.AttributeSet
import androidx.constraintlayout.widget.ConstraintLayout
import chat.pcp.app.R
import chat.pcp.app.databinding.ViewFailedMessagesWarningBinding

class FailedMessagesWarningView @JvmOverloads constructor(
        context: Context,
        attrs: AttributeSet? = null,
        defStyleAttr: Int = 0
) : ConstraintLayout(context, attrs, defStyleAttr) {

    interface Callback {
        fun onDeleteAllClicked()
        fun onRetryClicked()
    }

    var callback: Callback? = null

    private lateinit var views: ViewFailedMessagesWarningBinding

    init {
        setupViews()
    }

    private fun setupViews() {
        inflate(context, R.layout.view_failed_messages_warning, this)
        views = ViewFailedMessagesWarningBinding.bind(this)

        views.failedMessagesDeleteAllButton.setOnClickListener { callback?.onDeleteAllClicked() }
        views.failedMessagesRetryButton.setOnClickListener { callback?.onRetryClicked() }
    }
}
