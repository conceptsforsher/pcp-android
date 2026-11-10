/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.font

import com.airbnb.epoxy.TypedEpoxyController
import chat.pcp.app.core.epoxy.fontScaleItem
import chat.pcp.app.core.epoxy.fontScaleSectionItem
import chat.pcp.app.core.epoxy.fontScaleUseSystemSettingsItem
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.features.settings.FontScaleValue
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

class FontScaleSettingController @Inject constructor(
        val stringProvider: StringProvider
) : TypedEpoxyController<FontScaleSettingViewState>() {

    var callback: Callback? = null

    override fun buildModels(data: FontScaleSettingViewState?) {
        data?.let {
            buildAutomaticallySection(data.useSystemSettings)
            buildFontScaleItems(data.availableScaleOptions, data.persistedSettingIndex, data.useSystemSettings)
        }
    }

    private fun buildAutomaticallySection(useSystemSettings: Boolean) {
        val host = this
        fontScaleSectionItem {
            id("section_automatically")
            sectionName(host.stringProvider.getString(CommonStrings.font_size_section_auto))
        }

        fontScaleUseSystemSettingsItem {
            id("use_system_settings")
            useSystemSettings(useSystemSettings)
            checkChangeListener { _, isChecked ->
                host.callback?.onUseSystemSettingChanged(useSystemSettings = isChecked)
            }
        }
    }

    private fun buildFontScaleItems(scales: List<FontScaleValue>, persistedSettingIndex: Int, useSystemSettings: Boolean) {
        val host = this
        fontScaleSectionItem {
            id("section_manually")
            sectionName(host.stringProvider.getString(CommonStrings.font_size_section_manually))
        }

        scales.forEachIndexed { index, scaleItem ->
            fontScaleItem {
                id(scaleItem.index)
                fontScale(scaleItem)
                selected(index == persistedSettingIndex)
                enabled(!useSystemSettings)
                checkChangeListener { _, isChecked ->
                    if (isChecked) {
                        host.callback?.oFontScaleSelected(fonScale = scaleItem)
                    }
                }
            }
        }
    }

    interface Callback {
        fun onUseSystemSettingChanged(useSystemSettings: Boolean)
        fun oFontScaleSelected(fonScale: FontScaleValue)
    }
}
