/*
 * Copyright 2020-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.qrcode

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.activity.result.ActivityResultLauncher
import com.airbnb.mvrx.viewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.replaceFragment
import chat.pcp.app.core.platform.PcpActivity
import chat.pcp.app.databinding.ActivitySimpleBinding
import chat.pcp.lib.strings.CommonStrings

@AndroidEntryPoint
class QrCodeScannerActivity : PcpActivity<ActivitySimpleBinding>() {

    override fun getBinding() = ActivitySimpleBinding.inflate(layoutInflater)

    override fun getCoordinatorLayout() = views.coordinatorLayout

    override val rootView: View
        get() = views.coordinatorLayout

    private val qrViewModel: QrCodeScannerViewModel by viewModel()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        qrViewModel.observeViewEvents {
            when (it) {
                is QrCodeScannerEvents.CodeParsed -> {
                    setResultAndFinish(it.result, it.isQrCode)
                }
                is QrCodeScannerEvents.ParseFailed -> {
                    Toast.makeText(this, CommonStrings.qr_code_not_scanned, Toast.LENGTH_SHORT).show()
                    finish()
                }
                else -> Unit
            }
        }

        if (isFirstCreation()) {
            val args = QrScannerArgs(showExtraButtons = false, CommonStrings.verification_scan_their_code)
            replaceFragment(views.simpleFragmentContainer, QrCodeScannerFragment::class.java, args)
        }
    }

    private fun setResultAndFinish(result: String, isQrCode: Boolean) {
        setResult(RESULT_OK, Intent().apply {
            putExtra(EXTRA_OUT_TEXT, result)
            putExtra(EXTRA_OUT_IS_QR_CODE, isQrCode)
        })
        finish()
    }

    companion object {
        private const val EXTRA_OUT_TEXT = "EXTRA_OUT_TEXT"
        private const val EXTRA_OUT_IS_QR_CODE = "EXTRA_OUT_IS_QR_CODE"

        fun startForResult(activity: Activity, activityResultLauncher: ActivityResultLauncher<Intent>) {
            activityResultLauncher.launch(Intent(activity, QrCodeScannerActivity::class.java))
        }

        fun getResultText(data: Intent?): String? {
            return data?.getStringExtra(EXTRA_OUT_TEXT)
        }

        fun getResultIsQrCode(data: Intent?): Boolean {
            return data?.getBooleanExtra(EXTRA_OUT_IS_QR_CODE, false) == true
        }
    }
}
