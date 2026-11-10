/*
 * Copyright 2026, 2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.crypto.recover

import android.view.LayoutInflater
import android.view.ViewGroup
import com.airbnb.mvrx.parentFragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.extensions.setTextOrHide
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentBootstrapErrorBinding
import chat.pcp.lib.strings.CommonStrings

@AndroidEntryPoint
class BootstrapErrorFragment :
        PcpFragment<FragmentBootstrapErrorBinding>() {

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentBootstrapErrorBinding {
        return FragmentBootstrapErrorBinding.inflate(inflater, container, false)
    }

    val sharedViewModel: BootstrapSharedViewModel by parentFragmentViewModel()

    override fun invalidate() = withState(sharedViewModel) { state ->
        when (state.step) {
            is BootstrapStep.Error -> {
                views.bootstrapDescriptionText.setTextOrHide(errorFormatter.toHumanReadable(state.step.error))
            }
            else -> {
                // Should not happen, show a generic error
                views.bootstrapDescriptionText.setTextOrHide(getString(CommonStrings.unknown_error))
            }
        }
        views.bootstrapRetryButton.onClick {
            sharedViewModel.handle(BootstrapActions.Retry)
        }
    }
}
