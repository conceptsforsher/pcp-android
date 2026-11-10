/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.signout.hard

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.databinding.ActivitySignedOutBinding
import chat.pcp.app.features.MainActivity
import chat.pcp.app.features.MainActivityArgs
import org.matrix.android.sdk.api.failure.GlobalError
import timber.log.Timber

/**
 * In this screen, the user is viewing a message informing that he has been logged out.
 */
@AndroidEntryPoint
class SignedOutActivity : PcpActivity<ActivitySignedOutBinding>() {

    override fun getBinding() = ActivitySignedOutBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.signedOut

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setupViews()
    }

    private fun setupViews() {
        views.signedOutSubmit.debouncedClicks { submit() }
    }

    private fun submit() {
        // All is already cleared when we are here
        MainActivity.restartApp(this, MainActivityArgs())
    }

    companion object {
        fun newIntent(context: Context): Intent {
            return Intent(context, SignedOutActivity::class.java)
        }
    }

    override fun handleInvalidToken(globalError: GlobalError.InvalidToken) {
        // No op here
        Timber.w("Ignoring invalid token global error")
    }
}
