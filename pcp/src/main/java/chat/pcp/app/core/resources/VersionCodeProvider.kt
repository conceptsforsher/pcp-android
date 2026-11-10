/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.resources

import android.content.Context
import android.os.Build
import androidx.annotation.NonNull
import org.matrix.android.sdk.api.util.getPackageInfoCompat
import javax.inject.Inject

class VersionCodeProvider @Inject constructor(private val context: Context) {

    /**
     * Returns the version code, read from the Manifest. It is not the same than BuildConfig.VERSION_CODE due to versionCodeOverride
     */
    @NonNull
    fun getVersionCode(): Long {
        val packageInfo = context.packageManager.getPackageInfoCompat(context.packageName, 0)

        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            packageInfo.longVersionCode
        } else {
            @Suppress("DEPRECATION")
            packageInfo.versionCode.toLong()
        }
    }
}
