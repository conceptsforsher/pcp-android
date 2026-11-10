/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.epoxy

import android.os.Bundle
import android.os.Parcelable
import androidx.recyclerview.widget.RecyclerView
import chat.pcp.app.core.platform.DefaultListUpdateCallback
import chat.pcp.app.core.platform.Restorable
import chat.pcp.lib.core.utils.compat.getParcelableCompat
import java.util.concurrent.atomic.AtomicReference

private const val LAYOUT_MANAGER_STATE = "LAYOUT_MANAGER_STATE"

class LayoutManagerStateRestorer(layoutManager: RecyclerView.LayoutManager) : Restorable, DefaultListUpdateCallback {

    private var layoutManager: RecyclerView.LayoutManager? = null
    private var layoutManagerState = AtomicReference<Parcelable?>()

    init {
        this.layoutManager = layoutManager
    }

    fun clear() {
        layoutManager = null
    }

    override fun onSaveInstanceState(outState: Bundle) {
        val layoutManagerState = layoutManager?.onSaveInstanceState()
        outState.putParcelable(LAYOUT_MANAGER_STATE, layoutManagerState)
    }

    override fun onRestoreInstanceState(savedInstanceState: Bundle?) {
        val parcelable = savedInstanceState?.getParcelableCompat<Parcelable>(LAYOUT_MANAGER_STATE)
        layoutManagerState.set(parcelable)
    }

    override fun onInserted(position: Int, count: Int) {
        layoutManagerState.getAndSet(null)?.also {
            layoutManager?.onRestoreInstanceState(it)
        }
    }
}
