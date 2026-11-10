/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices.v2.rename

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.ViewTreeObserver
import androidx.core.widget.doOnTextChanged
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.showKeyboard
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentSessionRenameBinding
import chat.pcp.app.features.settings.devices.v2.more.SessionLearnMoreBottomSheet
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

/**
 * Display the screen to rename a Session.
 */
@AndroidEntryPoint
class RenameSessionFragment :
        PcpFragment<FragmentSessionRenameBinding>() {

    private val viewModel: RenameSessionViewModel by fragmentViewModel()

    @Inject lateinit var viewNavigator: RenameSessionViewNavigator

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentSessionRenameBinding {
        return FragmentSessionRenameBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        observeViewEvents()
        initToolbar()
        initEditText()
        initSaveButton()
        initWithLastEditedName()
        initInfoView()
    }

    private fun initToolbar() {
        setupToolbar(views.renameSessionToolbar)
                .allowBack(useCross = true)
    }

    private fun initEditText() {
        showKeyboard()
        views.renameSessionEditText.doOnTextChanged { text, _, _, _ ->
            viewModel.handle(RenameSessionAction.EditLocally(text.toString()))
        }
    }

    private fun showKeyboard() {
        val focusChangeListener = object : ViewTreeObserver.OnWindowFocusChangeListener {
            override fun onWindowFocusChanged(hasFocus: Boolean) {
                if (hasFocus) {
                    views.renameSessionEditText.showKeyboard(andRequestFocus = true)
                }
                views.renameSessionEditText.viewTreeObserver.removeOnWindowFocusChangeListener(this)
            }
        }
        views.renameSessionEditText.viewTreeObserver.addOnWindowFocusChangeListener(focusChangeListener)
    }

    private fun initSaveButton() {
        views.renameSessionSave.debouncedClicks {
            viewModel.handle(RenameSessionAction.SaveModifications)
        }
    }

    private fun initWithLastEditedName() {
        viewModel.handle(RenameSessionAction.InitWithLastEditedName)
    }

    private fun initInfoView() {
        views.renameSessionInfo.onLearnMoreClickListener = {
            showLearnMoreInfo()
        }
    }

    private fun showLearnMoreInfo() {
        val args = SessionLearnMoreBottomSheet.Args(
                title = getString(CommonStrings.device_manager_learn_more_session_rename_title),
                description = getString(CommonStrings.device_manager_learn_more_session_rename),
        )
        SessionLearnMoreBottomSheet
                .show(childFragmentManager, args)
                .onDismiss = { showKeyboard() }
    }

    private fun observeViewEvents() {
        viewModel.observeViewEvents {
            when (it) {
                is RenameSessionViewEvent.Initialized -> {
                    views.renameSessionEditText.setText(it.deviceName)
                    views.renameSessionEditText.setSelection(views.renameSessionEditText.length())
                }
                is RenameSessionViewEvent.SessionRenamed -> {
                    viewNavigator.goBack(requireActivity())
                }
                is RenameSessionViewEvent.Failure -> {
                    showFailure(it.throwable)
                }
            }
        }
    }

    override fun invalidate() = withState(viewModel) { state ->
        views.renameSessionSave.isEnabled = state.editedDeviceName.isNotEmpty()
    }
}
