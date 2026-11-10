/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.utils

import chat.pcp.app.core.resources.ColorProvider
import org.billcarsonfr.jsonviewer.JSonViewerStyleProvider

fun createJSonViewerStyleProvider(colorProvider: ColorProvider): JSonViewerStyleProvider {
    return JSonViewerStyleProvider(
            keyColor = colorProvider.getColorFromAttribute(com.google.android.material.R.attr.colorPrimary),
            secondaryColor = colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_content_secondary),
            stringColor = colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_notice_text_color),
            baseColor = colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_content_primary),
            booleanColor = colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_notice_text_color),
            numberColor = colorProvider.getColorFromAttribute(chat.pcp.lib.ui.styles.R.attr.vctr_notice_text_color)
    )
}
