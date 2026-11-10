/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.crypto.quads

import android.app.Activity
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.EditorInfo
import androidx.lifecycle.lifecycleScope
import com.airbnb.mvrx.activityViewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.registerStartForActivityResult
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.utils.startImportTextFromFileIntent
import chat.pcp.app.databinding.FragmentSsssAccessFromKeyBinding
import chat.pcp.lib.core.utils.flow.throttleFirst
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach
import org.matrix.android.sdk.api.extensions.tryOrNull
import reactivecircus.flowbinding.android.widget.editorActionEvents
import reactivecircus.flowbinding.android.widget.textChanges

@AndroidEntryPoint
class SharedSecuredStorageKeyFragment :
        PcpFragment<FragmentSsssAccessFromKeyBinding>() {

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentSsssAccessFromKeyBinding {
        return FragmentSsssAccessFromKeyBinding.inflate(inflater, container, false)
    }

    val sharedViewModel: SharedSecureStorageViewModel by activityViewModel()

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        views.ssssRestoreWithKeyText.text = getString(CommonStrings.enter_secret_storage_input_key)

        views.ssssKeyEnterEdittext.editorActionEvents()
                .throttleFirst(300)
                .onEach {
                    if (it.actionId == EditorInfo.IME_ACTION_DONE) {
                        submit()
                    }
                }
                .launchIn(viewLifecycleOwner.lifecycleScope)

        views.ssssKeyEnterEdittext.textChanges()
                .skipInitialValue()
                .onEach {
                    views.ssssKeyEnterTil.error = null
                    views.ssssKeySubmit.isEnabled = it.isNotBlank()
                }
                .launchIn(viewLifecycleOwner.lifecycleScope)

        views.ssssKeyUseFile.debouncedClicks { startImportTextFromFileIntent(requireContext(), importFileStartForActivityResult) }

        views.ssssKeyReset.views.bottomSheetActionClickableZone.debouncedClicks {
            sharedViewModel.handle(SharedSecureStorageAction.ForgotResetAll)
        }

        sharedViewModel.observeViewEvents {
            when (it) {
                is SharedSecureStorageViewEvent.KeyInlineError -> {
                    views.ssssKeyEnterTil.error = it.message
                }
                else -> Unit
            }
        }

        views.ssssKeySubmit.debouncedClicks { submit() }
    }

    fun submit() {
        val text = views.ssssKeyEnterEdittext.text.toString()
        if (text.isBlank()) return // Should not reach this point as button disabled
        views.ssssKeySubmit.isEnabled = false
        sharedViewModel.handle(SharedSecureStorageAction.SubmitKey(text))
    }

    private val importFileStartForActivityResult = registerStartForActivityResult { activityResult ->
        if (activityResult.resultCode == Activity.RESULT_OK) {
            activityResult.data?.data?.let { dataURI ->
                tryOrNull {
                    activity?.contentResolver?.openInputStream(dataURI)
                            ?.bufferedReader()
                            ?.use { it.readText() }
                            ?.let {
                                views.ssssKeyEnterEdittext.setText(it)
                            }
                }
            }
        }
    }
}
