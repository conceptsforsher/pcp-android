/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.matrixto

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isInvisible
import androidx.core.view.isVisible
import com.airbnb.mvrx.Fail
import com.airbnb.mvrx.Loading
import com.airbnb.mvrx.Success
import com.airbnb.mvrx.Uninitialized
import com.airbnb.mvrx.parentFragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.setTextOrHide
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentMatrixToUserCardBinding
import chat.pcp.app.features.home.AvatarRenderer
import javax.inject.Inject

@AndroidEntryPoint
class MatrixToUserFragment :
        PcpFragment<FragmentMatrixToUserCardBinding>() {

    @Inject lateinit var avatarRenderer: AvatarRenderer

    private val sharedViewModel: MatrixToBottomSheetViewModel by parentFragmentViewModel()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentMatrixToUserCardBinding {
        return FragmentMatrixToUserCardBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        views.matrixToCardSendMessageButton.debouncedClicks {
            withState(sharedViewModel) {
                it.matrixItem.invoke()?.let { item ->
                    sharedViewModel.handle(MatrixToAction.StartChattingWithUser(item))
                }
            }
        }
    }

    override fun invalidate() = withState(sharedViewModel) { state ->
        when (val item = state.matrixItem) {
            Uninitialized -> {
                views.matrixToCardUserContentVisibility.isVisible = false
            }
            is Loading -> {
                views.matrixToCardUserContentVisibility.isVisible = false
            }
            is Success -> {
                views.matrixToCardUserContentVisibility.isVisible = true
                views.matrixToCardNameText.setTextOrHide(item.invoke().displayName)
                views.matrixToCardUserIdText.setTextOrHide(item.invoke().id)
                avatarRenderer.render(item.invoke(), views.matrixToCardAvatar)
            }
            is Fail -> {
                // Error handled by sharedViewModel (dismisses sheet silently)
                sharedViewModel.handle(MatrixToAction.FailedToResolveUser)
            }
        }

        when (state.startChattingState) {
            Uninitialized -> {
                views.matrixToCardButtonLoading.isVisible = false
                views.matrixToCardSendMessageButton.isVisible = false
            }
            is Success -> {
                views.matrixToCardButtonLoading.isVisible = false
                views.matrixToCardSendMessageButton.isVisible = true
            }
            is Fail -> {
                views.matrixToCardButtonLoading.isVisible = false
                views.matrixToCardSendMessageButton.isVisible = true
                // Error handled by sharedViewModel (dismisses sheet silently)
                sharedViewModel.handle(MatrixToAction.FailedToStartChatting)
            }
            is Loading -> {
                views.matrixToCardButtonLoading.isVisible = true
                views.matrixToCardSendMessageButton.isInvisible = true
            }
        }
    }
}
