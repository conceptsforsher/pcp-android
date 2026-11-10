/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import chat.pcp.app.core.utils.BehaviorDataSource
import org.matrix.android.sdk.api.session.room.model.SpaceChildInfo
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class CurrentSpaceSuggestedRoomListDataSource @Inject constructor() : BehaviorDataSource<List<SpaceChildInfo>>()
