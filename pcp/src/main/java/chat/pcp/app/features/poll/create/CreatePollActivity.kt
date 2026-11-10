/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.poll.create

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.addFragment
import chat.pcp.app.core.platform.SimpleFragmentActivity
import chat.pcp.lib.core.utils.compat.getParcelableCompat

@AndroidEntryPoint
class CreatePollActivity : SimpleFragmentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        views.toolbar.visibility = View.GONE

        val createPollArgs: CreatePollArgs? = intent?.extras?.getParcelableCompat(EXTRA_CREATE_POLL_ARGS)

        if (isFirstCreation()) {
            addFragment(
                    views.container,
                    CreatePollFragment::class.java,
                    createPollArgs
            )
        }
    }

    companion object {

        private const val EXTRA_CREATE_POLL_ARGS = "EXTRA_CREATE_POLL_ARGS"

        fun getIntent(context: Context, createPollArgs: CreatePollArgs): Intent {
            return Intent(context, CreatePollActivity::class.java).apply {
                putExtra(EXTRA_CREATE_POLL_ARGS, createPollArgs)
            }
        }
    }
}
