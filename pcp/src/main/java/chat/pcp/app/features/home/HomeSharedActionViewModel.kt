/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home

import chat.pcp.app.core.platform.PcpSharedAction
import chat.pcp.app.core.platform.PcpSharedActionViewModel
import org.matrix.android.sdk.api.session.Session
import javax.inject.Inject

class HomeSharedActionViewModel @Inject constructor(val session: Session) : PcpSharedActionViewModel<HomeActivitySharedAction>()
