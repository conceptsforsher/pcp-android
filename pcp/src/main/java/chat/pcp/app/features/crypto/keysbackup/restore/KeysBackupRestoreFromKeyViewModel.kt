/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.crypto.keysbackup.restore

import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import chat.pcp.app.core.platform.WaitingViewData
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.session.crypto.keysbackup.BackupUtils
import javax.inject.Inject

class KeysBackupRestoreFromKeyViewModel @Inject constructor(
        private val stringProvider: StringProvider
) : ViewModel() {

    var recoveryCode: MutableLiveData<String?> = MutableLiveData(null)
    var recoveryCodeErrorText: MutableLiveData<String?> = MutableLiveData(null)

    // ========= Actions =========
    fun updateCode(newValue: String) {
        recoveryCode.value = newValue
        recoveryCodeErrorText.value = null
    }

    fun recoverKeys(sharedViewModel: KeysBackupRestoreSharedViewModel) {
        sharedViewModel.loadingEvent.postValue(WaitingViewData(stringProvider.getString(CommonStrings.loading)))
        recoveryCodeErrorText.value = null
        viewModelScope.launch(Dispatchers.IO) {
            try {
                val recoveryKey = BackupUtils.recoveryKeyFromBase58(recoveryCode.value!!)
                sharedViewModel.recoverUsingBackupRecoveryKey(recoveryKey)
            } catch (failure: Throwable) {
                recoveryCodeErrorText.postValue(stringProvider.getString(CommonStrings.keys_backup_recovery_code_error_decrypt))
            }
        }
    }
}
