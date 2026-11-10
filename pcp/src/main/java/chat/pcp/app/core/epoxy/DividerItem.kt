/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.core.epoxy

import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R

@EpoxyModelClass
abstract class DividerItem : PcpEpoxyModel<DividerItem.Holder>(R.layout.item_divider) {
    class Holder : PcpEpoxyHolder()
}
