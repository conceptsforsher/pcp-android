/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.contacts

import android.net.Uri

class MappedContactBuilder(
        val id: Long,
        val displayName: String
) {
    var photoURI: Uri? = null
    val msisdns = mutableListOf<MappedMsisdn>()
    val emails = mutableListOf<MappedEmail>()

    fun build(): MappedContact {
        return MappedContact(
                id = id,
                displayName = displayName,
                photoURI = photoURI,
                msisdns = msisdns,
                emails = emails
        )
    }
}

data class MappedContact(
        val id: Long,
        val displayName: String,
        val photoURI: Uri? = null,
        val msisdns: List<MappedMsisdn> = emptyList(),
        val emails: List<MappedEmail> = emptyList()
)

data class MappedEmail(
        val email: String,
        val matrixId: String?
)

data class MappedMsisdn(
        val phoneNumber: String,
        val matrixId: String?
)
