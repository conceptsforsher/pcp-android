/*
 * Copyright 2020-2024 PCP
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

/**
 * A action for bottom sheet.
 */
@EpoxyModelClass
abstract class BottomSheetVerificationDecimalCodeItem : PcpEpoxyModel<BottomSheetVerificationDecimalCodeItem.Holder>(
        R.layout.item_verification_decimal_code
) {

    @EpoxyAttribute
    var code: String = ""

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.code.text = code
    }

    class Holder : PcpEpoxyHolder() {
        val code by bind<TextView>(R.id.itemVerificationDecimalCode)
    }
}
