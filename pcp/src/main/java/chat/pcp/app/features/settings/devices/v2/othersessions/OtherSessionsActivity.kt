/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.othersessions

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import com.airbnb.mvrx.Mavericks
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.addFragment
import chat.pcp.app.core.platform.SimpleFragmentActivity
import chat.pcp.app.features.settings.devices.v2.filter.DeviceManagerFilterType
import chat.pcp.lib.core.utils.compat.getParcelableExtraCompat

@AndroidEntryPoint
class OtherSessionsActivity : SimpleFragmentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        views.toolbar.visibility = View.GONE

        if (isFirstCreation()) {
            addFragment(
                    container = views.container,
                    fragmentClass = OtherSessionsFragment::class.java,
                    params = intent.getParcelableExtraCompat(Mavericks.KEY_ARG)
            )
        }
    }

    companion object {
        fun newIntent(
                context: Context,
                defaultFilter: DeviceManagerFilterType,
                excludeCurrentDevice: Boolean,
        ): Intent {
            return Intent(context, OtherSessionsActivity::class.java).apply {
                putExtra(Mavericks.KEY_ARG, OtherSessionsArgs(defaultFilter, excludeCurrentDevice))
            }
        }
    }
}
