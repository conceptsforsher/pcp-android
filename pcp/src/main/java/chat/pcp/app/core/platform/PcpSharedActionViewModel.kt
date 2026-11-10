/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.platform

import androidx.lifecycle.ViewModel
import chat.pcp.app.core.utils.MutableDataSource
import chat.pcp.app.core.utils.PublishDataSource

interface PcpSharedAction

/**
 * Parent class to handle navigation events, action events, or other any events.
 */
open class PcpSharedActionViewModel<T : PcpSharedAction>(private val store: MutableDataSource<T> = PublishDataSource()) :
        ViewModel(), MutableDataSource<T> by store
