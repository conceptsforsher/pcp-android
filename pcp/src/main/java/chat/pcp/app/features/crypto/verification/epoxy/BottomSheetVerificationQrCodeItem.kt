/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.crypto.verification.epoxy

import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.ui.views.QrCodeImageView

/**
 * An Epoxy item displaying a QR code.
 */
@EpoxyModelClass
abstract class BottomSheetVerificationQrCodeItem : PcpEpoxyModel<BottomSheetVerificationQrCodeItem.Holder>(R.layout.item_verification_qr_code) {

    @EpoxyAttribute
    lateinit var data: String

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.qsrCodeImage.setData(data)
    }

    class Holder : PcpEpoxyHolder() {
        val qsrCodeImage by bind<QrCodeImageView>(R.id.itemVerificationQrCodeImage)
    }
}
