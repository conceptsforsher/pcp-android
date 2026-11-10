/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.date

import android.content.Context
import android.text.format.DateFormat
import chat.pcp.app.core.resources.LocaleProvider
import org.threeten.bp.format.DateTimeFormatter
import javax.inject.Inject

class DefaultDateFormatterProvider @Inject constructor(
        private val context: Context,
        private val localeProvider: LocaleProvider
) :
        DateFormatterProvider {

    override val dateWithMonthFormatter: DateTimeFormatter by lazy {
        val pattern = DateFormat.getBestDateTimePattern(localeProvider.current(), "d MMMMM")
        DateTimeFormatter.ofPattern(pattern)
    }

    override val dateWithYearFormatter: DateTimeFormatter by lazy {
        val pattern = DateFormat.getBestDateTimePattern(localeProvider.current(), "d MMM y")
        DateTimeFormatter.ofPattern(pattern)
    }
}
