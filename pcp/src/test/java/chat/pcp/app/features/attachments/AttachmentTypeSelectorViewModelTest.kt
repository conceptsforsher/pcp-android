/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.attachments

import com.airbnb.mvrx.test.MavericksTestRule
import chat.pcp.app.test.fakes.FakePcpFeatures
import chat.pcp.app.test.fakes.FakePcpPreferences
import chat.pcp.app.test.test
import io.mockk.verifyOrder
import org.junit.Before
import org.junit.Rule
import org.junit.Test

internal class AttachmentTypeSelectorViewModelTest {

    @get:Rule
    val mavericksTestRule = MavericksTestRule()

    private val fakePcpFeatures = FakePcpFeatures()
    private val fakePcpPreferences = FakePcpPreferences()
    private val initialState = AttachmentTypeSelectorViewState()

    @Before
    fun setUp() {
        // Disable all features by default
        fakePcpFeatures.givenLocationSharing(isEnabled = false)
        fakePcpFeatures.givenVoiceBroadcast(isEnabled = false)
        fakePcpPreferences.givenTextFormatting(isEnabled = false)
    }

    @Test
    fun `given features are not enabled, then options are not visible`() {
        createViewModel()
                .test()
                .assertStates(
                        listOf(
                                initialState,
                        )
                )
                .finish()
    }

    @Test
    fun `given location sharing is enabled, then location sharing option is visible`() {
        fakePcpFeatures.givenLocationSharing(isEnabled = true)

        createViewModel()
                .test()
                .assertStates(
                        listOf(
                                initialState.copy(
                                        isLocationVisible = true
                                ),
                        )
                )
                .finish()
    }

    @Test
    fun `given voice broadcast is enabled, then voice broadcast option is visible`() {
        fakePcpFeatures.givenVoiceBroadcast(isEnabled = true)

        createViewModel()
                .test()
                .assertStates(
                        listOf(
                                initialState.copy(
                                        isVoiceBroadcastVisible = true
                                ),
                        )
                )
                .finish()
    }

    @Test
    fun `given text formatting is enabled, then text formatting option is checked`() {
        fakePcpPreferences.givenTextFormatting(isEnabled = true)

        createViewModel()
                .test()
                .assertStates(
                        listOf(
                                initialState.copy(
                                        isTextFormattingEnabled = true
                                ),
                        )
                )
                .finish()
    }

    @Test
    fun `when text formatting is changed, then it updates the UI`() {
        createViewModel()
                .apply {
                    handle(AttachmentTypeSelectorAction.ToggleTextFormatting(isEnabled = true))
                }
                .test()
                .assertStates(
                        listOf(
                                initialState.copy(
                                        isTextFormattingEnabled = true
                                ),
                        )
                )
                .finish()
    }

    @Test
    fun `when text formatting is changed, then it persists the change`() {
        createViewModel()
                .apply {
                    handle(AttachmentTypeSelectorAction.ToggleTextFormatting(isEnabled = true))
                    handle(AttachmentTypeSelectorAction.ToggleTextFormatting(isEnabled = false))
                }
        verifyOrder {
            fakePcpPreferences.instance.setTextFormattingEnabled(true)
            fakePcpPreferences.instance.setTextFormattingEnabled(false)
        }
    }

    private fun createViewModel(): AttachmentTypeSelectorViewModel {
        return AttachmentTypeSelectorViewModel(
                initialState,
                vectorFeatures = fakePcpFeatures,
                vectorPreferences = fakePcpPreferences.instance,
        )
    }
}
