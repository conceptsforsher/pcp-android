/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.locale

import com.airbnb.epoxy.TypedEpoxyController
import com.airbnb.mvrx.Fail
import com.airbnb.mvrx.Loading
import com.airbnb.mvrx.Success
import com.airbnb.mvrx.Uninitialized
import chat.pcp.app.core.epoxy.errorWithRetryItem
import chat.pcp.app.core.epoxy.loadingItem
import chat.pcp.app.core.epoxy.noResultItem
import chat.pcp.app.core.epoxy.profiles.profileSectionItem
import chat.pcp.app.core.error.ErrorFormatter
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.core.utils.safeCapitalize
import chat.pcp.app.features.settings.PcpLocale
import chat.pcp.app.features.settings.PcpBasePreferences
import chat.pcp.lib.strings.CommonStrings
import java.util.Locale
import javax.inject.Inject

class LocalePickerController @Inject constructor(
        private val pcpPreferences: PcpBasePreferences,
        private val stringProvider: StringProvider,
        private val errorFormatter: ErrorFormatter,
        private val vectorLocale: PcpLocale,
) : TypedEpoxyController<LocalePickerViewState>() {

    var listener: Listener? = null

    override fun buildModels(data: LocalePickerViewState?) {
        val list = data?.locales ?: return
        val currentLocale = data.currentLocale ?: return
        val host = this

        profileSectionItem {
            id("currentTitle")
            title(host.stringProvider.getString(CommonStrings.choose_locale_current_locale_title))
        }
        localeItem {
            id(currentLocale.toString())
            title(host.vectorLocale.localeToLocalisedString(currentLocale).safeCapitalize(currentLocale))
            if (host.pcpPreferences.developerMode()) {
                subtitle(host.vectorLocale.localeToLocalisedStringInfo(currentLocale))
            }
            clickListener { host.listener?.onUseCurrentClicked() }
        }
        profileSectionItem {
            id("otherTitle")
            title(host.stringProvider.getString(CommonStrings.choose_locale_other_locales_title))
        }
        when (list) {
            Uninitialized,
            is Loading -> {
                loadingItem {
                    id("loading")
                    loadingText(host.stringProvider.getString(CommonStrings.choose_locale_loading_locales))
                }
            }
            is Success ->
                if (list().isEmpty()) {
                    noResultItem {
                        id("noResult")
                        text(host.stringProvider.getString(CommonStrings.no_result_placeholder))
                    }
                } else {
                    list()
                            .filter { it.toString() != currentLocale.toString() }
                            .forEach { locale ->
                                localeItem {
                                    id(locale.toString())
                                    title(host.vectorLocale.localeToLocalisedString(locale).safeCapitalize(locale))
                                    if (host.pcpPreferences.developerMode()) {
                                        subtitle(host.vectorLocale.localeToLocalisedStringInfo(locale))
                                    }
                                    clickListener { host.listener?.onLocaleClicked(locale) }
                                }
                            }
                }
            is Fail ->
                errorWithRetryItem {
                    id("error")
                    text(host.errorFormatter.toHumanReadable(list.error))
                }
        }
    }

    interface Listener {
        fun onUseCurrentClicked()
        fun onLocaleClicked(locale: Locale)
    }
}
