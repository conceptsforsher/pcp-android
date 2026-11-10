/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.webview

import android.content.Context
import android.content.Intent
import android.view.View
import android.webkit.WebChromeClient
import android.webkit.WebView
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.databinding.ActivityVectorWebViewBinding
import chat.pcp.lib.core.utils.compat.getSerializableCompat
import org.matrix.android.sdk.api.session.Session

/**
 * This class is responsible for managing a WebView
 * It does also have a loading view and a toolbar
 * It relies on the PcpWebViewClient
 * This class shouldn't be extended. To add new behaviors, you might create a new WebViewMode and a new WebViewEventListener
 */
@AndroidEntryPoint
class PcpWebViewActivity : PcpActivity<ActivityVectorWebViewBinding>() {

    override fun getBinding() = ActivityVectorWebViewBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.mainRoot

    val session: Session by lazy {
        activeSessionHolder.getActiveSession()
    }

    override fun initUiAndData() {
        setupToolbar(views.webviewToolbar)
                .allowBack()
        waitingView = views.simpleWebviewLoader

        views.simpleWebview.settings.apply {
            // Enable Javascript
            javaScriptEnabled = true

            // Use WideViewport and Zoom out if there is no viewport defined
            useWideViewPort = true
            loadWithOverviewMode = true

            // Enable pinch to zoom without the zoom buttons
            builtInZoomControls = true

            // Allow use of Local Storage
            domStorageEnabled = true

            @Suppress("DEPRECATION")
            allowFileAccessFromFileURLs = true
            @Suppress("DEPRECATION")
            allowUniversalAccessFromFileURLs = true

            displayZoomControls = false
        }

        val cookieManager = android.webkit.CookieManager.getInstance()
        cookieManager.setAcceptThirdPartyCookies(views.simpleWebview, true)

        val url = intent.extras?.getString(EXTRA_URL) ?: return
        val title = intent.extras?.getString(EXTRA_TITLE, USE_TITLE_FROM_WEB_PAGE)
        if (title != USE_TITLE_FROM_WEB_PAGE) {
            setTitle(title)
        }

        val webViewMode = intent.extras?.getSerializableCompat<WebViewMode>(EXTRA_MODE)!!
        val eventListener = webViewMode.eventListener(this, session)
        views.simpleWebview.webViewClient = PcpWebViewClient(eventListener)
        views.simpleWebview.webChromeClient = object : WebChromeClient() {
            override fun onReceivedTitle(view: WebView, title: String) {
                if (title == USE_TITLE_FROM_WEB_PAGE) {
                    setTitle(title)
                }
            }
        }
        views.simpleWebview.loadUrl(url)
    }

    /* ==========================================================================================
     * UI event
     * ========================================================================================== */

    @Suppress("OVERRIDE_DEPRECATION")
    override fun onBackPressed() {
        if (views.simpleWebview.canGoBack()) {
            views.simpleWebview.goBack()
        } else {
            super.onBackPressed()
        }
    }

    /* ==========================================================================================
     * Companion
     * ========================================================================================== */

    companion object {
        private const val EXTRA_URL = "EXTRA_URL"
        private const val EXTRA_TITLE = "EXTRA_TITLE"
        private const val EXTRA_MODE = "EXTRA_MODE"

        private const val USE_TITLE_FROM_WEB_PAGE = ""

        fun getIntent(
                context: Context,
                url: String,
                title: String = USE_TITLE_FROM_WEB_PAGE,
                mode: WebViewMode = WebViewMode.DEFAULT
        ): Intent {
            return Intent(context, PcpWebViewActivity::class.java)
                    .apply {
                        putExtra(EXTRA_URL, url)
                        putExtra(EXTRA_TITLE, title)
                        putExtra(EXTRA_MODE, mode)
                    }
        }
    }
}
