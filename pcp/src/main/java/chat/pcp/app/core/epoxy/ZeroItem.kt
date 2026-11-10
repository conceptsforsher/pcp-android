/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy

import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R

/**
 * Item of size (0, 0).
 * It can be useful to avoid automatic scroll of RecyclerView with Epoxy controller, when the first valuable item changes.
 */
@EpoxyModelClass
abstract class ZeroItem : PcpEpoxyModel<ZeroItem.Holder>(R.layout.item_zero) {

    class Holder : PcpEpoxyHolder()
}
