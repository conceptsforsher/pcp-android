/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.raw.wellknown

enum class SecureBackupMethod {
    KEY,
    PASSPHRASE,
    KEY_OR_PASSPHRASE;

    val isKeyAvailable: Boolean get() = this == KEY || this == KEY_OR_PASSPHRASE
    val isPassphraseAvailable: Boolean get() = this == PASSPHRASE || this == KEY_OR_PASSPHRASE
}
