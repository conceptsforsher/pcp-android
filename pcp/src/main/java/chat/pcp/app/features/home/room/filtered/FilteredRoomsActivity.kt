/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.home.room.filtered

import android.app.AlertDialog
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import androidx.appcompat.widget.SearchView
import androidx.lifecycle.lifecycleScope
import chat.pcp.app.native.PcpNative
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.replaceFragment
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.databinding.ActivityFilteredRoomsBinding
import im.vector.app.features.analytics.plan.MobileScreen
import chat.pcp.app.features.home.RoomListDisplayMode
import chat.pcp.app.features.home.room.list.RoomListFragment
import chat.pcp.app.features.home.room.list.RoomListParams
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import okhttp3.MediaType.Companion.toMediaType
import okhttp3.Request
import okhttp3.RequestBody.Companion.toRequestBody
import org.json.JSONObject

@AndroidEntryPoint
class FilteredRoomsActivity : PcpActivity<ActivityFilteredRoomsBinding>() {

    private val roomListFragment: RoomListFragment?
        get() {
            return supportFragmentManager.findFragmentByTag(FRAGMENT_TAG) as? RoomListFragment
        }

    override fun getBinding() = ActivityFilteredRoomsBinding.inflate(layoutInflater)

    override fun getCoordinatorLayout() = views.coordinatorLayout

    override val rootView: View
        get() = views.coordinatorLayout

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.RoomFilter
        setupToolbar(views.filteredRoomsToolbar)
                .allowBack()
        if (isFirstCreation()) {
            val params = RoomListParams(RoomListDisplayMode.FILTERED)
            replaceFragment(views.filteredRoomsFragmentContainer, RoomListFragment::class.java, params, FRAGMENT_TAG)
        }
        views.filteredRoomsSearchView.setOnQueryTextListener(object : SearchView.OnQueryTextListener {
            override fun onQueryTextSubmit(query: String): Boolean {
                if (handleLlmQuery(query)) return true
                return true
            }

            override fun onQueryTextChange(newText: String): Boolean {
                if (newText.startsWith("?")) return true
                roomListFragment?.filterRoomsWith(newText)
                return true
            }
        })
        // Open the keyboard immediately
        views.filteredRoomsSearchView.requestFocus()
    }

    private fun handleLlmQuery(query: String): Boolean {
        if (!query.startsWith("? ")) return false
        if (!pcpPreferences.isLlmSlashEnabled()) return false
        val prompt = query.removePrefix("? ").trim()
        if (prompt.isEmpty()) return false

        lifecycleScope.launch {
            val result = queryLlm(prompt)
            withContext(Dispatchers.Main) {
                AlertDialog.Builder(this@FilteredRoomsActivity)
                    .setTitle("LLM Response")
                    .setMessage(result)
                    .setPositiveButton("OK", null)
                    .show()
            }
        }
        return true
    }

    private suspend fun queryLlm(prompt: String): String {
        return withContext(Dispatchers.IO) {
            try {
                PcpNative.ensureLoaded()
                val provider = pcpPreferences.getLlmProvider()
                val endpoint = pcpPreferences.getLlmEndpoint()
                val token = pcpPreferences.getLlmToken()
                val model = pcpPreferences.getLlmModel()

                val body = PcpNative.nativeBuildLlmRequest(
                    prompt, provider, endpoint, token, model, "", 0.7f, 1024
                )
                val headers = PcpNative.nativeBuildLlmHeaders(provider, token)

                val jsonMedia = "application/json".toMediaType()
                val requestBody = body.toRequestBody(jsonMedia)
                val requestBuilder = Request.Builder()
                    .url(endpoint)
                    .post(requestBody)
                    .addHeader("Content-Type", "application/json")
                // Parse headers as key: value lines
                for (line in headers.split("\n")) {
                    val colon = line.indexOf(": ")
                    if (colon > 0) {
                        val key = line.substring(0, colon).trim()
                        val value = line.substring(colon + 2).trim()
                        requestBuilder.addHeader(key, value)
                    }
                }
                val request = requestBuilder.build()

                val session = activeSessionHolder.getActiveSession()
                if (session == null) return@withContext "No active session"
                val client = session.getOkHttpClient()
                val response = client.newCall(request).execute()
                val responseBody = response.body?.string() ?: ""
                val statusCode = response.code
                response.close()

                val parsed = PcpNative.nativeParseLlmResponse(responseBody, statusCode, provider)
                val json = JSONObject(parsed)
                if (json.getBoolean("success")) json.getString("text")
                else json.getString("errorMessage")
            } catch (e: Exception) {
                "LLM request failed: ${e.message}"
            }
        }
    }

    companion object {
        private const val FRAGMENT_TAG = "RoomListFragment"

        fun newIntent(context: Context): Intent {
            return Intent(context, FilteredRoomsActivity::class.java)
        }
    }
}
