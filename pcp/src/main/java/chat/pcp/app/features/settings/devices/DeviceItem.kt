/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.devices

import android.graphics.Typeface
import android.view.ViewGroup
import android.widget.TextView
import androidx.core.view.isVisible
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.resources.ColorProvider
import chat.pcp.app.core.ui.views.ShieldImageView
import chat.pcp.app.core.utils.DimensionConverter
import chat.pcp.lib.strings.CommonStrings
import me.gujun.android.span.span
import org.matrix.android.sdk.api.session.crypto.crosssigning.DeviceTrustLevel
import org.matrix.android.sdk.api.session.crypto.model.DeviceInfo

/**
 * A list item for Device.
 */
@EpoxyModelClass
abstract class DeviceItem : PcpEpoxyModel<DeviceItem.Holder>(R.layout.item_device) {

    @EpoxyAttribute
    lateinit var deviceInfo: DeviceInfo

    @EpoxyAttribute
    var lastSeenFormatted: String? = null

    @EpoxyAttribute
    var currentDevice = false

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var itemClickAction: ClickListener? = null

    @EpoxyAttribute
    var detailedMode = false

    @EpoxyAttribute
    var trusted: DeviceTrustLevel? = null

    @EpoxyAttribute
    var e2eCapable: Boolean = true

    @EpoxyAttribute
    var legacyMode: Boolean = false

    @EpoxyAttribute
    var trustedSession: Boolean = false

    @EpoxyAttribute
    var colorProvider: ColorProvider? = null

    @EpoxyAttribute
    var dimensionConverter: DimensionConverter? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.root.onClick(itemClickAction)

        if (e2eCapable) {
            val shield = TrustUtils.shieldForTrust(
                    currentDevice,
                    trustedSession,
                    legacyMode,
                    trusted
            )

            holder.trustIcon.renderDeviceShield(shield)
        } else {
            holder.trustIcon.renderDeviceShield(null)
        }

        val detailedModeLabels = listOf(
                holder.displayNameLabelText,
                holder.displayNameText,
                holder.deviceIdLabelText,
                holder.deviceIdText,
                holder.deviceLastSeenLabelText,
                holder.deviceLastSeenText
        )
        if (detailedMode) {
            holder.summaryLabelText.isVisible = false

            holder.displayNameText.text = deviceInfo.displayName ?: ""
            holder.deviceIdText.text = deviceInfo.deviceId ?: ""

            val lastSeenIp = deviceInfo.lastSeenIp?.takeIf { ip -> ip.isNotBlank() } ?: "-"

            val lastSeenTime = lastSeenFormatted ?: "-"

            holder.deviceLastSeenText.text = holder.root.context.getString(CommonStrings.devices_details_last_seen_format, lastSeenIp, lastSeenTime)

            detailedModeLabels.map {
                it.isVisible = true
                it.setTypeface(null, if (currentDevice) Typeface.BOLD else Typeface.NORMAL)
            }
        } else {
            holder.summaryLabelText.text =
                    span {
                        +(deviceInfo.displayName ?: deviceInfo.deviceId ?: "")
                        apply {
                            // Add additional info if current session is not trusted
                            if (!trustedSession) {
                                +"\n"
                                span {
                                    text = "${deviceInfo.deviceId}"
                                    apply {
                                        colorProvider?.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_content_secondary)?.let {
                                            textColor = it
                                        }
                                        dimensionConverter?.spToPx(12)?.let {
                                            textSize = it
                                        }
                                    }
                                }
                            }
                        }
                    }

            holder.summaryLabelText.isVisible = true
            detailedModeLabels.map {
                it.isVisible = false
            }
        }
    }

    class Holder : PcpEpoxyHolder() {
        val root by bind<ViewGroup>(R.id.itemDeviceRoot)
        val summaryLabelText by bind<TextView>(R.id.itemDeviceSimpleSummary)
        val displayNameLabelText by bind<TextView>(R.id.itemDeviceDisplayNameLabel)
        val displayNameText by bind<TextView>(R.id.itemDeviceDisplayName)
        val deviceIdLabelText by bind<TextView>(R.id.itemDeviceIdLabel)
        val deviceIdText by bind<TextView>(R.id.itemDeviceId)
        val deviceLastSeenLabelText by bind<TextView>(R.id.itemDeviceLastSeenLabel)
        val deviceLastSeenText by bind<TextView>(R.id.itemDeviceLastSeen)

        val trustIcon by bind<ShieldImageView>(R.id.itemDeviceTrustLevelIcon)
    }
}
