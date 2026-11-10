/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test.fakes

import com.posthog.PostHogInterface
import chat.pcp.app.features.analytics.impl.PostHogFactory
import io.mockk.every
import io.mockk.mockk

class FakePostHogFactory(postHog: PostHogInterface) {
    val instance = mockk<PostHogFactory>().also {
        every { it.createPosthog() } returns postHog
    }
}
