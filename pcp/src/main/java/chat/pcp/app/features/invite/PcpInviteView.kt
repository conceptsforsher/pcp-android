/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.invite

import android.content.Context
import android.util.AttributeSet
import android.view.View
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.core.view.updateLayoutParams
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.databinding.PcpInviteViewBinding
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.session.room.members.ChangeMembershipState
import org.matrix.android.sdk.api.session.room.model.RoomMemberSummary
import org.matrix.android.sdk.api.util.toMatrixItem
import javax.inject.Inject

@AndroidEntryPoint
class PcpInviteView @JvmOverloads constructor(context: Context, attrs: AttributeSet? = null, defStyle: Int = 0) :
        ConstraintLayout(context, attrs, defStyle) {

    interface Callback {
        fun onAcceptInvite()
        fun onRejectInvite()
    }

    enum class Mode {
        LARGE,
        SMALL
    }

    private val views: PcpInviteViewBinding

    @Inject lateinit var avatarRenderer: AvatarRenderer
    var callback: Callback? = null

    init {
        inflate(context, R.layout.pcp_invite_view, this)
        views = PcpInviteViewBinding.bind(this)
        views.inviteAcceptView.commonClicked = { callback?.onAcceptInvite() }
        views.inviteRejectView.commonClicked = { callback?.onRejectInvite() }
    }

    fun render(sender: RoomMemberSummary, mode: Mode = Mode.LARGE, changeMembershipState: ChangeMembershipState) {
        if (mode == Mode.LARGE) {
            updateLayoutParams { height = LayoutParams.MATCH_CONSTRAINT }
            avatarRenderer.render(sender.toMatrixItem(), views.inviteAvatarView)
            views.inviteIdentifierView.text = sender.userId
            views.inviteNameView.text = sender.displayName
            views.inviteLabelView.text = context.getString(CommonStrings.send_you_invite)
        } else {
            updateLayoutParams { height = LayoutParams.WRAP_CONTENT }
            views.inviteAvatarView.visibility = View.GONE
            views.inviteIdentifierView.visibility = View.GONE
            views.inviteNameView.visibility = View.GONE
            views.inviteLabelView.text = context.getString(CommonStrings.invited_by, sender.userId)
        }
        InviteButtonStateBinder.bind(views.inviteAcceptView, views.inviteRejectView, changeMembershipState)
    }
}
