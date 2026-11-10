/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.settings.labs

import com.airbnb.mvrx.test.MavericksTestRule
import chat.pcp.app.core.session.clientinfo.DeleteMatrixClientInfoUseCase
import chat.pcp.app.core.session.clientinfo.UpdateMatrixClientInfoUseCase
import chat.pcp.app.test.fakes.FakeActiveSessionHolder
import chat.pcp.app.test.test
import chat.pcp.app.test.testDispatcher
import io.mockk.coEvery
import io.mockk.coVerify
import io.mockk.mockk
import org.junit.Rule
import org.junit.Test

class PcpSettingsLabsTest {

    @get:Rule
    val mavericksTestRule = MavericksTestRule(testDispatcher = testDispatcher)

    private val fakeActiveSessionHolder = FakeActiveSessionHolder()
    private val fakeUpdateMatrixClientInfoUseCase = mockk<UpdateMatrixClientInfoUseCase>()
    private val fakeDeleteMatrixClientInfoUseCase = mockk<DeleteMatrixClientInfoUseCase>()

    private fun createViewModel(): VectorSettingsLabsViewModel {
        return VectorSettingsLabsViewModel(
                initialState = VectorSettingsLabsViewState(),
                activeSessionHolder = fakeActiveSessionHolder.instance,
                updateMatrixClientInfoUseCase = fakeUpdateMatrixClientInfoUseCase,
                deleteMatrixClientInfoUseCase = fakeDeleteMatrixClientInfoUseCase,
        )
    }

    @Test
    fun `given update client info action when handling this action then update client info use case is called`() {
        // Given
        givenUpdateClientInfoSucceeds()

        // When
        val viewModel = createViewModel()
        val viewModelTest = viewModel.test()
        viewModel.handle(VectorSettingsLabsAction.UpdateClientInfo)

        // Then
        viewModelTest.finish()
        coVerify { fakeUpdateMatrixClientInfoUseCase.execute(fakeActiveSessionHolder.fakeSession) }
    }

    @Test
    fun `given delete client info action when handling this action then delete client info use case is called`() {
        // Given
        givenDeleteClientInfoSucceeds()

        // When
        val viewModel = createViewModel()
        val viewModelTest = viewModel.test()
        viewModel.handle(VectorSettingsLabsAction.DeleteRecordedClientInfo)

        // Then
        viewModelTest.finish()
        coVerify { fakeDeleteMatrixClientInfoUseCase.execute() }
    }

    private fun givenUpdateClientInfoSucceeds() {
        coEvery { fakeUpdateMatrixClientInfoUseCase.execute(any()) } returns Result.success(Unit)
    }

    private fun givenDeleteClientInfoSucceeds() {
        coEvery { fakeDeleteMatrixClientInfoUseCase.execute() } returns Result.success(Unit)
    }
}
