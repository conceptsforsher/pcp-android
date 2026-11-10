/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.extensions

import android.content.Context
import androidx.core.content.ContextCompat
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.ProcessLifecycleOwner
import chat.pcp.app.core.services.PcpSyncService
import chat.pcp.app.features.session.PcpSessionStore
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.crypto.keysbackup.KeysBackupState
import timber.log.Timber

fun Session.startSyncing(context: Context) {
    val applicationContext = context.applicationContext

    // PCP: native sync parser deferred to v0.2
    // (requires careful integration with Labs flag + file-size safety checks)

    if (!syncService().hasAlreadySynced()) {
        // initial sync is done as a service so it can continue below app lifecycle
        PcpSyncService.newOneShotIntent(
                context = applicationContext,
                sessionId = sessionId
        )
                .let {
                    try {
                        ContextCompat.startForegroundService(applicationContext, it)
                    } catch (ex: Throwable) {
                        // Foreground service start may fail if app is in background
                        // or if foreground service restrictions apply
                        Timber.e(ex, "Failed to start foreground sync service")
                    }
                }
    } else {
        val isAtLeastStarted = ProcessLifecycleOwner.get().lifecycle.currentState.isAtLeast(Lifecycle.State.STARTED)
        Timber.v("--> is at least started? $isAtLeastStarted")
        syncService().startSync(isAtLeastStarted)
    }
}

/**
 * Tell is the session has unsaved e2e keys in the backup.
 */
suspend fun Session.hasUnsavedKeys(): Boolean {
    return cryptoService().inboundGroupSessionsCount(false) > 0 &&
            cryptoService().keysBackupService().getState() != KeysBackupState.ReadyToBackUp
}

suspend fun Session.cannotLogoutSafely(): Boolean {
    // has some encrypted chat
    return hasUnsavedKeys() ||
            // has local cross signing keys
            (cryptoService().crossSigningService().allPrivateKeysKnown() &&
                    // That are not backed up
                    !sharedSecretStorageService().isRecoverySetup())
}

fun Session.vectorStore(context: Context) = PcpSessionStore(context, myUserId)
