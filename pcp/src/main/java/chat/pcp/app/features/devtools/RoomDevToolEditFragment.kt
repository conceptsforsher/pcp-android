/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.devtools

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.lifecycle.lifecycleScope
import com.airbnb.mvrx.activityViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.hideKeyboard
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentDevtoolsEditorBinding
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach
import reactivecircus.flowbinding.android.widget.textChanges

@AndroidEntryPoint
class RoomDevToolEditFragment :
        PcpFragment<FragmentDevtoolsEditorBinding>() {

    private val sharedViewModel: RoomDevToolViewModel by activityViewModel()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentDevtoolsEditorBinding {
        return FragmentDevtoolsEditorBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        withState(sharedViewModel) {
            views.editText.setText(it.editedContent ?: "{}")
        }
        views.editText.textChanges()
                .skipInitialValue()
                .onEach {
                    sharedViewModel.handle(RoomDevToolAction.UpdateContentText(it.toString()))
                }
                .launchIn(viewLifecycleOwner.lifecycleScope)
    }

    override fun onResume() {
        super.onResume()
        views.editText.requestFocus()
    }

    override fun onStop() {
        super.onStop()
        views.editText.hideKeyboard()
    }
}
