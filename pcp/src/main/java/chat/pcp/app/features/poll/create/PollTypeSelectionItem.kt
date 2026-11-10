/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.poll.create

import android.widget.RadioGroup
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import org.matrix.android.sdk.api.session.room.model.message.PollType

@EpoxyModelClass
abstract class PollTypeSelectionItem : PcpEpoxyModel<PollTypeSelectionItem.Holder>(R.layout.item_poll_type_selection) {

    @EpoxyAttribute
    var pollType: PollType = PollType.DISCLOSED_UNSTABLE

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var pollTypeChangedListener: RadioGroup.OnCheckedChangeListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)

        holder.pollTypeRadioGroup.check(
                when (pollType) {
                    PollType.DISCLOSED_UNSTABLE, PollType.DISCLOSED -> R.id.openPollTypeRadioButton
                    PollType.UNDISCLOSED_UNSTABLE, PollType.UNDISCLOSED -> R.id.closedPollTypeRadioButton
                }
        )

        holder.pollTypeRadioGroup.setOnCheckedChangeListener(pollTypeChangedListener)
    }

    override fun unbind(holder: Holder) {
        super.unbind(holder)
        holder.pollTypeRadioGroup.setOnCheckedChangeListener(null)
    }

    class Holder : PcpEpoxyHolder() {
        val pollTypeRadioGroup by bind<RadioGroup>(R.id.pollTypeRadioGroup)
    }
}
