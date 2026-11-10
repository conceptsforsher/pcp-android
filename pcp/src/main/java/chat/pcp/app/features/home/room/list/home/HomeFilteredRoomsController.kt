/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.list.home

import androidx.paging.PagedList
import com.airbnb.epoxy.EpoxyModel
import com.airbnb.epoxy.paging.PagedListEpoxyController
import chat.pcp.app.core.platform.StateView
import chat.pcp.app.core.utils.createUIHandler
import chat.pcp.app.features.home.RoomListDisplayMode
import chat.pcp.app.features.home.room.list.RoomListListener
import chat.pcp.app.features.home.room.list.RoomSummaryItemFactory
import chat.pcp.app.features.home.room.list.RoomSummaryPlaceHolderItem_
import chat.pcp.app.features.settings.FontScalePreferences
import org.matrix.android.sdk.api.session.room.ResultBoundaries
import org.matrix.android.sdk.api.session.room.members.ChangeMembershipState
import org.matrix.android.sdk.api.session.room.model.RoomSummary
import javax.inject.Inject

class HomeFilteredRoomsController @Inject constructor(
        private val roomSummaryItemFactory: RoomSummaryItemFactory,
        fontScalePreferences: FontScalePreferences,
        roomSummaryRoomListDiffCallback: RoomSummaryRoomListDiffCallback,
) : PagedListEpoxyController<RoomSummary>(
        // Important it must match the PageList builder notify Looper
        modelBuildingHandler = createUIHandler(),
        itemDiffCallback = roomSummaryRoomListDiffCallback,
) {

    private var roomChangeMembershipStates: Map<String, ChangeMembershipState>? = null
        set(value) {
            field = value
            // ideally we could search for visible models and update only those
            requestForcedModelBuild()
        }

    var listener: RoomListListener? = null

    private var emptyStateData: StateView.State.Empty? = null

    private val shouldUseSingleLine: Boolean

    var initialLoadOccurred = false

    init {
        val fontScale = fontScalePreferences.getResolvedFontScaleValue()
        shouldUseSingleLine = fontScale.scale > FontScalePreferences.SCALE_LARGE
    }

    fun submitRoomsList(roomsList: PagedList<RoomSummary>) {
        submitList(roomsList)
        // If room is empty we may have a new EmptyState to display
        if (roomsList.isEmpty()) {
            requestForcedModelBuild()
        }
    }

    override fun addModels(models: List<EpoxyModel<*>>) {
        val emptyStateData = this.emptyStateData
        if (models.isEmpty() && emptyStateData != null) {
            roomListEmptyItem {
                id("state_item")
                emptyData(emptyStateData)
            }
        } else {
            super.addModels(models)
        }
    }

    fun boundaryChange(boundary: ResultBoundaries) {
        // Sometimes the room stays on empty state, need
        val boundaryHasLoadedSomething = boundary.frontLoaded || boundary.zeroItemLoaded
        if (initialLoadOccurred != boundaryHasLoadedSomething) {
            initialLoadOccurred = boundaryHasLoadedSomething
            requestForcedModelBuild()
        }
    }

    fun submitEmptyStateData(state: StateView.State.Empty?) {
        this.emptyStateData = state
    }

    override fun buildItemModel(currentPosition: Int, item: RoomSummary?): EpoxyModel<*> {
        return if (item == null) {
            val host = this
            RoomSummaryPlaceHolderItem_().apply {
                id(currentPosition)
                useSingleLineForLastEvent(host.shouldUseSingleLine)
            }
        } else {
            roomSummaryItemFactory.create(
                    roomSummary = item,
                    roomChangeMembershipStates = roomChangeMembershipStates.orEmpty(),
                    selectedRoomIds = emptySet(),
                    displayMode = RoomListDisplayMode.ROOMS,
                    listener = listener,
                    singleLineLastEvent = shouldUseSingleLine
            )
        }
    }
}
