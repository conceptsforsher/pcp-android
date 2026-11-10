/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.ui.views

import android.content.Context
import android.util.AttributeSet
import android.view.View
import android.widget.ImageView
import android.widget.LinearLayout
import androidx.core.view.isVisible
import chat.pcp.app.R
import chat.pcp.app.databinding.ViewReadReceiptsBinding
import chat.pcp.app.features.home.AvatarRenderer
import chat.pcp.app.features.home.room.detail.timeline.item.ReadReceiptData
import chat.pcp.app.features.home.room.detail.timeline.item.toMatrixItem
import chat.pcp.lib.strings.CommonPlurals
import chat.pcp.lib.strings.CommonStrings

/**
 * Max visible read receipt avatars before showing "+N".
 * Default increased from 3 to 20 for PCP.
 * User-configurable via Labs setting.
 */
var maxReceiptDisplayed: Int = 20

class ReadReceiptsView @JvmOverloads constructor(
        context: Context,
        attrs: AttributeSet? = null,
        defStyleAttr: Int = 0
) : LinearLayout(context, attrs, defStyleAttr) {

    private val views: ViewReadReceiptsBinding

    init {
        setupView()
        views = ViewReadReceiptsBinding.bind(this)
    }

    // Dynamic avatar views for expanded limits
    private val dynamicAvatars = mutableListOf<ImageView>()

    private val receiptAvatars: List<ImageView> by lazy {
        listOf(views.receiptAvatar1, views.receiptAvatar2, views.receiptAvatar3)
    }

    private fun ensureAvatars(count: Int) {
        val avatarSize = (16 * resources.displayMetrics.density).toInt() // item_event_message_state_size
        while ((receiptAvatars.size + dynamicAvatars.size) < count) {
            val iv = ImageView(context).apply {
                layoutParams = LayoutParams(avatarSize, avatarSize)
                setPadding(0, 0, -(4 * resources.displayMetrics.density).toInt(), 0)
            }
            dynamicAvatars.add(iv)
            addView(iv, 0) // insert at beginning for right-to-left ordering
        }
    }

    private fun setupView() {
        inflate(context, R.layout.view_read_receipts, this)
        contentDescription = context.getString(CommonStrings.a11y_view_read_receipts)
    }

    fun render(readReceipts: List<ReadReceiptData>, avatarRenderer: AvatarRenderer) {
        val maxDisplay = maxReceiptDisplayed.coerceAtLeast(1)
        ensureAvatars(maxDisplay)

        // Hide all first
        receiptAvatars.forEach { it.isVisible = false }
        dynamicAvatars.forEach { it.isVisible = false }

        val allAvatars = receiptAvatars + dynamicAvatars

        readReceipts.take(maxDisplay).forEachIndexed { index, receiptData ->
            if (index < allAvatars.size) {
                allAvatars[index].isVisible = true
                avatarRenderer.render(receiptData.toMatrixItem(), allAvatars[index])
            }
        }

        val displayNames = readReceipts
                .mapNotNull { it.displayName }
                .filter { it.isNotBlank() }
                .take(maxDisplay)

        if (readReceipts.size > maxDisplay) {
            views.receiptMore.visibility = View.VISIBLE
            views.receiptMore.text = context.getString(
                    CommonStrings.x_plus, readReceipts.size - maxDisplay
            )
        } else {
            views.receiptMore.visibility = View.GONE
        }
        contentDescription = when (readReceipts.size) {
            1 ->
                if (displayNames.size == 1) {
                    context.getString(CommonStrings.one_user_read, displayNames[0])
                } else {
                    context.resources.getQuantityString(CommonPlurals.fallback_users_read, readReceipts.size)
                }
            2 ->
                if (displayNames.size == 2) {
                    context.getString(CommonStrings.two_users_read, displayNames[0], displayNames[1])
                } else {
                    context.resources.getQuantityString(CommonPlurals.fallback_users_read, readReceipts.size)
                }
            3 ->
                if (displayNames.size == 3) {
                    context.getString(CommonStrings.three_users_read, displayNames[0], displayNames[1], displayNames[2])
                } else {
                    context.resources.getQuantityString(CommonPlurals.fallback_users_read, readReceipts.size)
                }
            else ->
                if (displayNames.size >= 2) {
                    val qty = readReceipts.size - 2
                    context.resources.getQuantityString(
                            CommonPlurals.two_and_some_others_read,
                            qty,
                            displayNames[0],
                            displayNames[1],
                            qty
                    )
                } else {
                    context.resources.getQuantityString(CommonPlurals.fallback_users_read, readReceipts.size)
                }
        }
    }

    fun unbind(avatarRenderer: AvatarRenderer?) {
        receiptAvatars.forEach {
            avatarRenderer?.clear(it)
        }
        isVisible = false
    }
}
