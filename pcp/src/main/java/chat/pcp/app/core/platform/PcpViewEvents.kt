/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.platform

/**
 * Interface for View Events.
 */
interface PcpViewEvents

/**
 * To use when no view events is associated to the ViewModel.
 */
object EmptyViewEvents : PcpViewEvents
