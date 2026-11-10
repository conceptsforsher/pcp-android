/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.extensions

import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import chat.pcp.app.core.utils.EventObserver
import chat.pcp.app.core.utils.FirstThrottler
import chat.pcp.app.core.utils.LiveEvent

inline fun <T> LiveData<T>.observeK(owner: LifecycleOwner, crossinline observer: (T?) -> Unit) {
    this.observe(owner, { observer(it) })
}

inline fun <T> LiveData<T>.observeNotNull(owner: LifecycleOwner, crossinline observer: (T) -> Unit) {
    this.observe(owner, { it?.run(observer) })
}

inline fun <T> LiveData<LiveEvent<T>>.observeEvent(owner: LifecycleOwner, crossinline observer: (T) -> Unit) {
    this.observe(owner, EventObserver { it.run(observer) })
}

inline fun <T> LiveData<LiveEvent<T>>.observeEventFirstThrottle(owner: LifecycleOwner, minimumInterval: Long, crossinline observer: (T) -> Unit) {
    val firstThrottler = FirstThrottler(minimumInterval)

    this.observe(owner, EventObserver {
        if (firstThrottler.canHandle() is FirstThrottler.CanHandlerResult.Yes) {
            it.run(observer)
        }
    })
}

fun <T> MutableLiveData<LiveEvent<T>>.postLiveEvent(content: T) {
    this.postValue(LiveEvent(content))
}
