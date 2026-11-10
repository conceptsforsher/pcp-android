/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.settings.crosssigning

import com.airbnb.epoxy.TypedEpoxyController
import chat.pcp.app.R
import chat.pcp.app.core.resources.ColorProvider
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.ui.list.genericButtonItem
import chat.pcp.app.core.ui.list.genericItem
import chat.pcp.app.core.ui.list.genericPositiveButtonItem
import chat.pcp.app.core.ui.list.genericWithValueItem
import chat.pcp.app.core.utils.DimensionConverter
import chat.pcp.lib.core.utils.epoxy.charsequence.toEpoxyCharSequence
import chat.pcp.lib.strings.CommonStrings
import me.gujun.android.span.span
import javax.inject.Inject

class CrossSigningSettingsController @Inject constructor(
        private val stringProvider: StringProvider,
        private val colorProvider: ColorProvider,
        private val dimensionConverter: DimensionConverter
) : TypedEpoxyController<CrossSigningSettingsViewState>() {

    interface InteractionListener {
        fun didTapInitializeCrossSigning()
    }

    var interactionListener: InteractionListener? = null

    override fun buildModels(data: CrossSigningSettingsViewState?) {
        if (data == null) return
        val host = this
        when {
            data.xSigningKeyCanSign -> {
                genericItem {
                    id("can")
                    titleIconResourceId(R.drawable.ic_shield_trusted)
                    title(host.stringProvider.getString(CommonStrings.encryption_information_dg_xsigning_complete).toEpoxyCharSequence())
                }
                genericButtonItem {
                    id("Reset")
                    text(host.stringProvider.getString(CommonStrings.reset_cross_signing))
                    buttonClickAction {
                        host.interactionListener?.didTapInitializeCrossSigning()
                    }
                }
            }
            data.xSigningKeysAreTrusted -> {
                genericItem {
                    id("trusted")
                    titleIconResourceId(R.drawable.ic_shield_custom)
                    title(host.stringProvider.getString(CommonStrings.encryption_information_dg_xsigning_trusted).toEpoxyCharSequence())
                }
                genericButtonItem {
                    id("Reset")
                    text(host.stringProvider.getString(CommonStrings.reset_cross_signing))
                    buttonClickAction {
                        host.interactionListener?.didTapInitializeCrossSigning()
                    }
                }
            }
            data.xSigningIsEnableInAccount -> {
                genericItem {
                    id("enable")
                    titleIconResourceId(R.drawable.ic_shield_black)
                    title(host.stringProvider.getString(CommonStrings.encryption_information_dg_xsigning_not_trusted).toEpoxyCharSequence())
                }
                genericButtonItem {
                    id("Reset")
                    text(host.stringProvider.getString(CommonStrings.reset_cross_signing))
                    buttonClickAction {
                        host.interactionListener?.didTapInitializeCrossSigning()
                    }
                }
            }
            else -> {
                genericItem {
                    id("not")
                    title(host.stringProvider.getString(CommonStrings.encryption_information_dg_xsigning_disabled).toEpoxyCharSequence())
                }

                genericPositiveButtonItem {
                    id("Initialize")
                    text(host.stringProvider.getString(CommonStrings.initialize_cross_signing))
                    buttonClickAction {
                        host.interactionListener?.didTapInitializeCrossSigning()
                    }
                }
            }
        }

        val crossSigningKeys = data.crossSigningInfo

        crossSigningKeys?.masterKey()?.let {
            genericWithValueItem {
                id("msk")
                titleIconResourceId(R.drawable.key_small)
                title(
                        span {
                            +"Master Key:\n"
                            span {
                                text = it.unpaddedBase64PublicKey ?: ""
                                textColor = host.colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_content_secondary)
                                textSize = host.dimensionConverter.spToPx(12)
                            }
                        }.toEpoxyCharSequence()
                )
            }
        }
        crossSigningKeys?.userKey()?.let {
            genericWithValueItem {
                id("usk")
                titleIconResourceId(R.drawable.key_small)
                title(
                        span {
                            +"User Key:\n"
                            span {
                                text = it.unpaddedBase64PublicKey ?: ""
                                textColor = host.colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_content_secondary)
                                textSize = host.dimensionConverter.spToPx(12)
                            }
                        }.toEpoxyCharSequence()
                )
            }
        }
        crossSigningKeys?.selfSigningKey()?.let {
            genericWithValueItem {
                id("ssk")
                titleIconResourceId(R.drawable.key_small)
                title(
                        span {
                            +"Self Signed Key:\n"
                            span {
                                text = it.unpaddedBase64PublicKey ?: ""
                                textColor = host.colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_content_secondary)
                                textSize = host.dimensionConverter.spToPx(12)
                            }
                        }.toEpoxyCharSequence()
                )
            }
        }
    }
}
