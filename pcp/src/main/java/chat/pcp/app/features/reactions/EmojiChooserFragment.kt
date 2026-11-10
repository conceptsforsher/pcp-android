/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.reactions

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.lifecycle.lifecycleScope
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.core.extensions.cleanup
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.databinding.EmojiChooserFragmentBinding
import javax.inject.Inject

@AndroidEntryPoint
class EmojiChooserFragment :
        PcpFragment<EmojiChooserFragmentBinding>(),
        EmojiRecyclerAdapter.InteractionListener,
        ReactionClickListener {

    @Inject lateinit var emojiRecyclerAdapter: EmojiRecyclerAdapter

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): EmojiChooserFragmentBinding {
        return EmojiChooserFragmentBinding.inflate(inflater, container, false)
    }

    private lateinit var viewModel: EmojiChooserViewModel

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        viewModel = activityViewModelProvider.get(EmojiChooserViewModel::class.java)
        emojiRecyclerAdapter.reactionClickListener = this
        emojiRecyclerAdapter.interactionListener = this
        views.emojiRecyclerView.adapter = emojiRecyclerAdapter
        viewModel.moveToSection.observe(viewLifecycleOwner) { section ->
            emojiRecyclerAdapter.scrollToSection(section)
        }
        viewModel.emojiData.observe(viewLifecycleOwner) {
            emojiRecyclerAdapter.update(it)
        }
    }

    override fun getCoroutineScope() = lifecycleScope

    override fun firstVisibleSectionChange(section: Int) {
        viewModel.setCurrentSection(section)
    }

    override fun onReactionSelected(reaction: String) {
        viewModel.onReactionSelected(reaction)
    }

    override fun onDestroyView() {
        views.emojiRecyclerView.cleanup()
        emojiRecyclerAdapter.reactionClickListener = null
        emojiRecyclerAdapter.interactionListener = null
        super.onDestroyView()
    }
}
