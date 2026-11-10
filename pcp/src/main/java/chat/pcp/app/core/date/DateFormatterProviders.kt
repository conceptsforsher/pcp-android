/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.date

import javax.inject.Inject

class DateFormatterProviders @Inject constructor(
        private val defaultDateFormatterProvider: DefaultDateFormatterProvider,
        private val abbrevDateFormatterProvider: AbbrevDateFormatterProvider
) {

    fun provide(abbrev: Boolean): DateFormatterProvider {
        return if (abbrev) {
            abbrevDateFormatterProvider
        } else {
            defaultDateFormatterProvider
        }
    }
}
