/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.crypto.keysbackup.restore

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.utils.LiveEvent
import chat.pcp.app.databinding.FragmentKeysBackupRestoreSuccessBinding
import chat.pcp.lib.strings.CommonPlurals
import chat.pcp.lib.strings.CommonStrings

@AndroidEntryPoint
class KeysBackupRestoreSuccessFragment :
        PcpFragment<FragmentKeysBackupRestoreSuccessBinding>() {

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentKeysBackupRestoreSuccessBinding {
        return FragmentKeysBackupRestoreSuccessBinding.inflate(inflater, container, false)
    }

    private lateinit var sharedViewModel: KeysBackupRestoreSharedViewModel

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        sharedViewModel = activityViewModelProvider.get(KeysBackupRestoreSharedViewModel::class.java)

        if (compareValues(sharedViewModel.importKeyResult?.totalNumberOfKeys, 0) > 0) {
            sharedViewModel.importKeyResult?.let {
                val part1 = resources.getQuantityString(
                        CommonPlurals.keys_backup_restore_success_description_part1,
                        it.totalNumberOfKeys, it.totalNumberOfKeys
                )
                val part2 = resources.getQuantityString(
                        CommonPlurals.keys_backup_restore_success_description_part2,
                        it.successfullyNumberOfImportedKeys, it.successfullyNumberOfImportedKeys
                )
                views.successDetailsText.text = String.format("%s\n%s", part1, part2)
            }
            // We don't put emoji in string xml as it will crash on old devices
            views.successText.text = context?.getString(CommonStrings.keys_backup_restore_success_title, "🎉")
        } else {
            views.successText.text = context?.getString(CommonStrings.keys_backup_restore_success_title_already_up_to_date)
            views.successDetailsText.isVisible = false
        }
        views.keysBackupSetupDoneButton.debouncedClicks { onDone() }
    }

    private fun onDone() {
        sharedViewModel.importRoomKeysFinishWithResult.value = LiveEvent(sharedViewModel.importKeyResult!!)
    }
}
