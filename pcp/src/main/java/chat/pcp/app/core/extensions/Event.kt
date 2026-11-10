/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.extensions

import chat.pcp.app.core.resources.DateProvider
import org.matrix.android.sdk.api.session.events.model.Event
import org.threeten.bp.LocalDateTime

fun Event.localDateTime(): LocalDateTime {
    return DateProvider.toLocalDateTime(originServerTs)
}
