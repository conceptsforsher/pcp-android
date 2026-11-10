/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.media

import com.github.piasy.biv.loader.ImageLoader
import java.io.File

interface DefaultImageLoaderCallback : ImageLoader.Callback {

    override fun onFinish() {
        // no-op
    }

    override fun onSuccess(image: File?) {
        // no-op
    }

    override fun onFail(error: Exception?) {
        // no-op
    }

    override fun onCacheHit(imageType: Int, image: File?) {
        // no-op
    }

    override fun onCacheMiss(imageType: Int, image: File?) {
        // no-op
    }

    override fun onProgress(progress: Int) {
        // no-op
    }

    override fun onStart() {
        // no-op
    }
}
