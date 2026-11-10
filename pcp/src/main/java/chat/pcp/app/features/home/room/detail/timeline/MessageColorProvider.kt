/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.detail.timeline

import androidx.annotation.ColorInt
import chat.pcp.app.core.resources.ColorProvider
import chat.pcp.app.features.home.room.detail.timeline.helper.MatrixItemColorProvider
import chat.pcp.app.features.settings.PcpBasePreferences
import org.matrix.android.sdk.api.session.room.send.SendState
import org.matrix.android.sdk.api.util.MatrixItem
import javax.inject.Inject

class MessageColorProvider @Inject constructor(
        private val colorProvider: ColorProvider,
        private val matrixItemColorProvider: MatrixItemColorProvider,
        private val pcpPreferences: PcpBasePreferences
) {

    @ColorInt
    fun getMemberNameTextColor(matrixItem: MatrixItem): Int {
        return matrixItemColorProvider.getColor(matrixItem)
    }

    @ColorInt
    fun getMessageTextColor(sendState: SendState): Int {
        return if (pcpPreferences.developerMode()) {
            when (sendState) {
                // SendStates, in the classical order they will occur
                SendState.UNKNOWN,
                SendState.UNSENT -> colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_sending_message_text_color)
                SendState.ENCRYPTING -> colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_encrypting_message_text_color)
                SendState.SENDING -> colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_sending_message_text_color)
                SendState.SENT,
                SendState.SYNCED -> colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_message_text_color)
                SendState.UNDELIVERED,
                SendState.FAILED_UNKNOWN_DEVICES -> colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_unsent_message_text_color)
            }
        } else {
            // When not in developer mode, we use only one color
            colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_message_text_color)
        }
    }
}
