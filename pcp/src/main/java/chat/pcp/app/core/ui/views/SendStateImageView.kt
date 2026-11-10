/*
 * Copyright 2021-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.ui.views

import android.content.Context
import android.content.res.ColorStateList
import android.util.AttributeSet
import androidx.appcompat.widget.AppCompatImageView
import androidx.core.view.isInvisible
import chat.pcp.app.R
import chat.pcp.app.features.home.room.detail.timeline.item.SendStateDecoration
import chat.pcp.app.features.themes.ThemeUtils
import chat.pcp.lib.strings.CommonStrings

class SendStateImageView @JvmOverloads constructor(
        context: Context,
        attrs: AttributeSet? = null,
        defStyleAttr: Int = 0
) : AppCompatImageView(context, attrs, defStyleAttr) {

    init {
        if (isInEditMode) {
            render(SendStateDecoration.SENT)
        }
    }

    fun render(sendState: SendStateDecoration) {
        isInvisible = when (sendState) {
            SendStateDecoration.SENDING_NON_MEDIA -> {
                setImageResource(R.drawable.ic_sending_message)
                imageTintList = ColorStateList.valueOf(ThemeUtils.getColor(context, chat.pcp.lib.ui.styles.R.attr.vctr_content_tertiary))
                contentDescription = context.getString(CommonStrings.event_status_a11y_sending)
                false
            }
            SendStateDecoration.SENT -> {
                setImageResource(R.drawable.ic_message_sent)
                imageTintList = ColorStateList.valueOf(ThemeUtils.getColor(context, chat.pcp.lib.ui.styles.R.attr.vctr_content_tertiary))
                contentDescription = context.getString(CommonStrings.event_status_a11y_sent)
                false
            }
            SendStateDecoration.FAILED -> {
                setImageResource(R.drawable.ic_sending_message_failed)
                imageTintList = null
                contentDescription = context.getString(CommonStrings.event_status_a11y_failed)
                false
            }
            SendStateDecoration.SENDING_MEDIA,
            SendStateDecoration.NONE -> {
                true
            }
        }
    }
}
