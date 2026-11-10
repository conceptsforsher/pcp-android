/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import android.graphics.drawable.Drawable
import chat.pcp.app.features.home.room.detail.timeline.helper.LocationPinProvider
import io.mockk.every
import io.mockk.invoke
import io.mockk.mockk
import org.matrix.android.sdk.api.util.MatrixItem

class FakeLocationPinProvider {

    val instance = mockk<LocationPinProvider>(relaxed = true)

    fun givenCreateForMatrixItem(matrixItem: MatrixItem, expectedDrawable: Drawable) {
        every { instance.create(matrixItem, captureLambda()) } answers { lambda<(Drawable) -> Unit>().invoke(expectedDrawable) }
    }
}
