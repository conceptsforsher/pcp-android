/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.crypto.verification.epoxy

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.lib.core.utils.epoxy.charsequence.EpoxyCharSequence

/**
 * A action for bottom sheet.
 */
@EpoxyModelClass
abstract class BottomSheetVerificationNoticeItem : PcpEpoxyModel<BottomSheetVerificationNoticeItem.Holder>(R.layout.item_verification_notice) {

    @EpoxyAttribute
    lateinit var notice: EpoxyCharSequence

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.notice.text = notice.charSequence
    }

    class Holder : PcpEpoxyHolder() {
        val notice by bind<TextView>(R.id.itemVerificationNoticeText)
    }
}
