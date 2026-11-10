/*
 * Copyright 2026-2026 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.test

import com.airbnb.mvrx.MavericksState
import chat.pcp.app.core.platform.VectorViewEvents
import chat.pcp.app.core.platform.PcpViewModel
import chat.pcp.app.core.platform.PcpViewModelAction
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers

fun String.trimIndentOneLine() = trimIndent().replace("\n", "")

fun <S : MavericksState, VA : PcpViewModelAction, VE : VectorViewEvents> PcpViewModel<S, VA, VE>.test(): ViewModelTest<S, VE> {
    val testResultCollectingScope = CoroutineScope(Dispatchers.Unconfined)
    val state = stateFlow.test(testResultCollectingScope)
    val viewEvents = viewEvents.stream("test").test(testResultCollectingScope)
    return ViewModelTest(state, viewEvents)
}

class ViewModelTest<S, VE>(
        val states: FlowTestObserver<S>,
        val viewEvents: FlowTestObserver<VE>
) {

    fun assertNoEvents(): ViewModelTest<S, VE> {
        viewEvents.assertNoValues()
        return this
    }

    fun assertEvents(vararg expected: VE): ViewModelTest<S, VE> {
        viewEvents.assertValues(*expected)
        return this
    }

    fun assertEvent(position: Int = 0, predicate: (VE) -> Boolean): ViewModelTest<S, VE> {
        viewEvents.assertValue(position, predicate)
        return this
    }

    fun assertStates(vararg expected: S): ViewModelTest<S, VE> {
        states.assertValues(*expected)
        return this
    }

    fun assertStatesChanges(initial: S, vararg expected: S.() -> S): ViewModelTest<S, VE> {
        return assertStatesChanges(initial, expected.toList())
    }

    /**
     * Asserts the expected states are in the same order as the actual state emissions.
     * Each expected lambda is given the previous expected state, starting with the initial
     */
    fun assertStatesChanges(initial: S, expected: List<S.() -> S>): ViewModelTest<S, VE> {
        val reducedExpectedStates = expected.fold(mutableListOf(initial)) { acc, curr ->
            val next = curr.invoke(acc.last())
            acc.add(next)
            acc
        }

        states.assertValues(reducedExpectedStates)
        return this
    }

    fun assertStates(expected: List<S>): ViewModelTest<S, VE> {
        states.assertValues(expected)
        return this
    }

    fun assertState(expected: S): ViewModelTest<S, VE> {
        states.assertValues(expected)
        return this
    }

    fun assertLatestState(expected: S): ViewModelTest<S, VE> {
        states.assertLatestValue(expected)
        return this
    }

    fun assertLatestState(predicate: (S) -> Boolean): ViewModelTest<S, VE> {
        states.assertLatestValue(predicate)
        return this
    }

    fun finish() {
        states.finish()
        viewEvents.finish()
    }
}
