/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location.live.map

import android.content.Context
import android.content.Intent
import android.os.Parcelable
import android.view.View
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.addFragment
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.databinding.ActivityLocationSharingBinding
import chat.pcp.app.features.MainActivity
import chat.pcp.lib.core.utils.compat.getParcelableCompat
import chat.pcp.lib.strings.CommonStrings
import kotlinx.parcelize.Parcelize

@Parcelize
data class LiveLocationMapViewArgs(
        val roomId: String
) : Parcelable

@AndroidEntryPoint
class LiveLocationMapViewActivity : PcpActivity<ActivityLocationSharingBinding>() {

    override fun getBinding() = ActivityLocationSharingBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.mainRoot

    override fun initUiAndData() {
        val mapViewArgs: LiveLocationMapViewArgs? = intent?.extras?.getParcelableCompat(EXTRA_LIVE_LOCATION_MAP_VIEW_ARGS)
        if (mapViewArgs == null) {
            finish()
            return
        }
        setupToolbar(views.toolbar)
                .setTitle(getString(CommonStrings.location_activity_title_preview))
                .allowBack()

        if (isFirstCreation()) {
            addFragment(
                    views.fragmentContainer,
                    LiveLocationMapViewFragment::class.java,
                    mapViewArgs
            )
        }
    }

    companion object {

        private const val EXTRA_LIVE_LOCATION_MAP_VIEW_ARGS = "EXTRA_LIVE_LOCATION_MAP_VIEW_ARGS"

        fun getIntent(context: Context, liveLocationMapViewArgs: LiveLocationMapViewArgs, firstStartMainActivity: Boolean = false): Intent {
            val intent = Intent(context, LiveLocationMapViewActivity::class.java).apply {
                putExtra(EXTRA_LIVE_LOCATION_MAP_VIEW_ARGS, liveLocationMapViewArgs)
            }
            return if (firstStartMainActivity) {
                MainActivity.getIntentWithNextIntent(context, intent)
            } else {
                intent
            }
        }
    }
}
