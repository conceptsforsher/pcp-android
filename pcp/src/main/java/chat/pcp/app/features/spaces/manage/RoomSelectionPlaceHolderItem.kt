/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.spaces.manage

import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel

@EpoxyModelClass
abstract class RoomSelectionPlaceHolderItem : PcpEpoxyModel<RoomSelectionPlaceHolderItem.Holder>(R.layout.item_room_to_add_in_space_placeholder) {
    class Holder : PcpEpoxyHolder()
}
