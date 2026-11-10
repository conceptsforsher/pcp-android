/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.analytics.impl

import android.content.Context
import chat.pcp.app.features.analytics.AnalyticsConfig
import chat.pcp.app.features.analytics.errors.ErrorTracker
import chat.pcp.app.features.analytics.log.analyticsTag
import io.sentry.Sentry
import io.sentry.SentryOptions
import io.sentry.android.core.SentryAndroid
import timber.log.Timber
import javax.inject.Inject

class SentryAnalytics @Inject constructor(
        private val context: Context,
        private val analyticsConfig: AnalyticsConfig,
) : ErrorTracker {

    fun initSentry() {
        Timber.tag(analyticsTag.value).d("Initializing Sentry")
        if (Sentry.isEnabled()) return
        SentryAndroid.init(context) { options ->
            options.dsn = analyticsConfig.sentryDSN
            options.beforeSend = SentryOptions.BeforeSendCallback { event, _ -> event }
            options.tracesSampleRate = 1.0
            options.isEnableUserInteractionTracing = true
            options.environment = analyticsConfig.sentryEnvironment
            options.diagnosticLevel
        }
    }

    fun stopSentry() {
        Timber.tag(analyticsTag.value).d("Stopping Sentry")
        Sentry.close()
    }

    override fun trackError(throwable: Throwable) {
        Sentry.captureException(throwable)
    }
}
