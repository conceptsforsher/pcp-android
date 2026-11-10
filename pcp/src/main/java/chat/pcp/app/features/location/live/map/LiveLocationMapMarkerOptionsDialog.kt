/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location.live.map

import android.content.Context
import android.view.View
import android.view.ViewGroup
import android.widget.PopupWindow
import chat.pcp.app.R
import chat.pcp.app.databinding.ViewLiveLocationMarkerPopupBinding

class LiveLocationMapMarkerOptionsDialog(
        context: Context,
) : PopupWindow() {

    interface Callback {
        fun onShareLocationClicked()
    }

    private val views: ViewLiveLocationMarkerPopupBinding

    var callback: Callback? = null

    init {
        contentView = View.inflate(context, R.layout.view_live_location_marker_popup, null)

        views = ViewLiveLocationMarkerPopupBinding.bind(contentView)

        width = ViewGroup.LayoutParams.WRAP_CONTENT
        height = ViewGroup.LayoutParams.WRAP_CONTENT
        inputMethodMode = INPUT_METHOD_NOT_NEEDED
        isFocusable = true
        isTouchable = true

        contentView.setOnClickListener {
            callback?.onShareLocationClicked()
        }
    }

    fun show(anchorView: View) {
        contentView.measure(View.MeasureSpec.UNSPECIFIED, View.MeasureSpec.UNSPECIFIED)
        // By default the left side of the dialog is aligned with the pin. We need shift it to the left to make it's center aligned with the pin.
        showAsDropDown(anchorView, -contentView.measuredWidth / 2, 0)
    }
}
