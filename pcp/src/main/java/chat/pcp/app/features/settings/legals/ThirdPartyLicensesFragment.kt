/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.settings.legals

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.extensions.configureWith
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.FragmentGenericRecyclerBinding
import chat.pcp.lib.strings.CommonStrings
import javax.inject.Inject

/**
 * Displays third-party open source licenses natively in a RecyclerView
 * instead of loading open_source_licenses.html in a WebView.
 */
@AndroidEntryPoint
class ThirdPartyLicensesFragment :
    PcpFragment<FragmentGenericRecyclerBinding>() {

    @Inject lateinit var controller: ThirdPartyLicensesController

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentGenericRecyclerBinding {
        return FragmentGenericRecyclerBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        (activity as? AppCompatActivity)?.supportActionBar?.setTitle(CommonStrings.settings_third_party_notices)

        val licenses = loadLicenses()
        controller.setData(licenses)
        views.genericRecyclerView.configureWith(controller)
    }

    override fun onDestroyView() {
        views.genericRecyclerView.cleanup()
        super.onDestroyView()
    }

    private fun loadLicenses(): List<LicenseEntry> {
        val html = requireContext().assets.open("open_source_licenses.html")
            .bufferedReader().readText()
        return LicenseParser.parse(html)
    }
}
