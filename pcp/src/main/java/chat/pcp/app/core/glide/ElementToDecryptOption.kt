/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.glide

import com.bumptech.glide.load.Option
import org.matrix.android.sdk.api.session.crypto.attachments.ElementToDecrypt

const val ElementToDecryptOptionKey = "chat.pcp.app.core.glide.ElementToDecrypt"

val ELEMENT_TO_DECRYPT = Option.memory(
        ElementToDecryptOptionKey, ElementToDecrypt("", "", "")
)
