/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.call.transfer

import android.os.Bundle
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentActivity
import androidx.viewpager2.adapter.FragmentStateAdapter
import chat.pcp.app.core.extensions.toMvRxBundle
import chat.pcp.app.features.call.dialpad.DialPadFragment
import chat.pcp.app.features.settings.PcpLocaleProvider
import chat.pcp.app.features.userdirectory.UserListFragment
import chat.pcp.app.features.userdirectory.UserListFragmentArgs

class CallTransferPagerAdapter(
        private val fragmentActivity: FragmentActivity,
        private val vectorLocale: PcpLocaleProvider,
) : FragmentStateAdapter(fragmentActivity) {

    companion object {
        const val USER_LIST_INDEX = 0
        const val DIAL_PAD_INDEX = 1
    }

    val userListFragment: UserListFragment?
        get() = findFragmentAtPosition(USER_LIST_INDEX) as? UserListFragment
    val dialPadFragment: DialPadFragment?
        get() = findFragmentAtPosition(DIAL_PAD_INDEX) as? DialPadFragment

    override fun getItemCount() = 2

    override fun createFragment(position: Int): Fragment {
        val fragment: Fragment
        if (position == 0) {
            fragment = fragmentActivity.supportFragmentManager.fragmentFactory.instantiate(fragmentActivity.classLoader, UserListFragment::class.java.name)
            fragment.arguments = UserListFragmentArgs(
                    title = "",
                    menuResId = -1,
                    submitMenuItemId = -1,
                    singleSelection = true,
                    showInviteActions = false,
                    showToolbar = false,
                    showContactBookAction = false
            ).toMvRxBundle()
        } else {
            fragment = fragmentActivity.supportFragmentManager.fragmentFactory.instantiate(fragmentActivity.classLoader, DialPadFragment::class.java.name)
            (fragment as DialPadFragment).apply {
                arguments = Bundle().apply {
                    putBoolean(DialPadFragment.EXTRA_ENABLE_DELETE, true)
                    putBoolean(DialPadFragment.EXTRA_ENABLE_OK, false)
                    putString(DialPadFragment.EXTRA_REGION_CODE, vectorLocale.applicationLocale.country)
                }
            }
        }
        return fragment
    }

    private fun findFragmentAtPosition(position: Int): Fragment? {
        return fragmentActivity.supportFragmentManager.findFragmentByTag("f$position")
    }
}
