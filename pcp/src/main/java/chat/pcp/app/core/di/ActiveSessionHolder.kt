/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.di

import chat.pcp.app.ActiveSessionDataSource
import chat.pcp.app.core.dispatchers.CoroutineDispatchers
import chat.pcp.app.core.pushers.UnregisterUnifiedPushUseCase
import chat.pcp.app.core.services.GuardServiceStarter
import chat.pcp.app.core.session.ConfigureAndStartSessionUseCase
import chat.pcp.app.features.call.webrtc.WebRtcCallManager
import chat.pcp.app.features.crypto.keysrequest.KeyRequestHandler
import chat.pcp.app.features.crypto.verification.IncomingVerificationRequestHandler
import chat.pcp.app.features.notifications.PushRuleTriggerListener
import chat.pcp.app.features.session.SessionListener
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import org.matrix.android.sdk.api.auth.AuthenticationService
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.util.Optional
import org.matrix.android.sdk.api.util.toOption
import timber.log.Timber
import java.util.concurrent.atomic.AtomicReference
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class ActiveSessionHolder @Inject constructor(
        private val activeSessionDataSource: ActiveSessionDataSource,
        private val keyRequestHandler: KeyRequestHandler,
        private val incomingVerificationRequestHandler: IncomingVerificationRequestHandler,
        private val callManager: WebRtcCallManager,
        private val pushRuleTriggerListener: PushRuleTriggerListener,
        private val sessionListener: SessionListener,
        private val imageManager: ImageManager,
        private val guardServiceStarter: GuardServiceStarter,
        private val sessionInitializer: SessionInitializer,
        private val authenticationService: AuthenticationService,
        private val configureAndStartSessionUseCase: ConfigureAndStartSessionUseCase,
        private val unregisterUnifiedPushUseCase: UnregisterUnifiedPushUseCase,
        private val applicationCoroutineScope: CoroutineScope,
        private val coroutineDispatchers: CoroutineDispatchers,
) {

    private var activeSessionReference: AtomicReference<Session?> = AtomicReference()

    fun setActiveSession(session: Session) {
        Timber.w("setActiveSession of ${session.myUserId}")
        activeSessionReference.set(session)
        activeSessionDataSource.post(session.toOption())

        keyRequestHandler.start(session)
        incomingVerificationRequestHandler.start(session)
        session.addListener(sessionListener)
        pushRuleTriggerListener.startWithSession(session)
        session.callSignalingService().addCallListener(callManager)
        imageManager.onSessionStarted(session)
        guardServiceStarter.start()
    }

    suspend fun clearActiveSession() {
        // Do some cleanup first
        getSafeActiveSession()?.let {
            Timber.w("clearActiveSession of ${it.myUserId}")
            it.callSignalingService().removeCallListener(callManager)
            it.removeListener(sessionListener)
        }

        activeSessionReference.set(null)
        activeSessionDataSource.post(Optional.empty())

        keyRequestHandler.stop()
        incomingVerificationRequestHandler.stop()
        pushRuleTriggerListener.stop()
        // No need to unregister the pusher, the sign out will (should?) do it server side.
        unregisterUnifiedPushUseCase.execute(pushersManager = null)
        guardServiceStarter.stop()
    }

    fun hasActiveSession(): Boolean {
        return activeSessionReference.get() != null || authenticationService.hasAuthenticatedSessions()
    }

    fun getSafeActiveSession(): Session? {
        return runBlocking { getOrInitializeSession() }
    }

    fun getSafeActiveSessionAsync(withSession: ((Session?) -> Unit)) {
        applicationCoroutineScope.launch(coroutineDispatchers.io) {
            val session = getOrInitializeSession()
            withSession(session)
        }
    }

    fun getActiveSession(): Session {
        return getSafeActiveSession()
                ?: throw IllegalStateException("You should authenticate before using this")
    }

    suspend fun getOrInitializeSession(): Session? {
        return activeSessionReference.get()
                ?: sessionInitializer.tryInitialize(readCurrentSession = { activeSessionReference.get() }) { session ->
                    setActiveSession(session)
                    configureAndStartSessionUseCase.execute(session, startSyncing = false)
                }
    }

    fun isWaitingForSessionInitialization() = activeSessionReference.get() == null && authenticationService.hasAuthenticatedSessions()

    // TODO Stop sync ?
//    fun switchToSession(sessionParams: SessionParams) {
//        val newActiveSession = authenticationService.getSession(sessionParams)
//        activeSession.set(newActiveSession)
//    }
}
