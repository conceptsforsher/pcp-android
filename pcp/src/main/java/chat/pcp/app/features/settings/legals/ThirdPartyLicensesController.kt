/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.settings.legals

import com.airbnb.epoxy.TypedEpoxyController
import chat.pcp.app.features.discovery.settingsSectionTitleItem
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

class ThirdPartyLicensesController @Inject constructor() :
    TypedEpoxyController<List<LicenseEntry>>() {

    override fun buildModels(licenses: List<LicenseEntry>) {
        settingsSectionTitleItem {
            id("title")
            titleResId(CommonStrings.settings_third_party_notices)
        }

        licenses.forEachIndexed { index, license ->
            licenseItem {
                id("license_$index")
                name(license.name)
                copyright(license.copyright)
                licenseText(license.licenseText)
            }
        }
    }
}
