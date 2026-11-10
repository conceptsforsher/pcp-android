/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.create

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.mvrx.activityViewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentSpaceCreateChooseTypeBinding
import im.vector.app.features.analytics.plan.MobileScreen

@AndroidEntryPoint
class ChooseSpaceTypeFragment :
        PcpFragment<FragmentSpaceCreateChooseTypeBinding>() {

    private val sharedViewModel: CreateSpaceViewModel by activityViewModel()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?) =
            FragmentSpaceCreateChooseTypeBinding.inflate(layoutInflater, container, false)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.CreateSpace
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        views.publicButton.onClick {
            sharedViewModel.handle(CreateSpaceAction.SetRoomType(SpaceType.Public))
        }

        views.privateButton.onClick {
            sharedViewModel.handle(CreateSpaceAction.SetRoomType(SpaceType.Private))
        }
    }
}
