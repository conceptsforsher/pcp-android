/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.lib.multipicker.entity

interface MultiPickerBaseMediaType : MultiPickerBaseType {
    val width: Int
    val height: Int
    val orientation: Int
}
