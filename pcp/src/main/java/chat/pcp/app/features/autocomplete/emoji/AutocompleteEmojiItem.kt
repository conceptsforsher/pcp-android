/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.autocomplete.emoji

import android.graphics.Typeface
import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.extensions.setTextOrHide
import chat.pcp.app.features.reactions.data.EmojiItem

@EpoxyModelClass
abstract class AutocompleteEmojiItem : PcpEpoxyModel<AutocompleteEmojiItem.Holder>(R.layout.item_autocomplete_emoji) {

    @EpoxyAttribute
    lateinit var emojiItem: EmojiItem

    @EpoxyAttribute
    var emojiTypeFace: Typeface? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var onClickListener: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.emojiText.text = emojiItem.emoji
        holder.emojiText.typeface = emojiTypeFace ?: Typeface.DEFAULT
        holder.emojiNameText.text = emojiItem.name
        holder.emojiKeywordText.setTextOrHide(emojiItem.keywords.joinToString())
        holder.view.onClick(onClickListener)
    }

    class Holder : PcpEpoxyHolder() {
        val emojiText by bind<TextView>(R.id.itemAutocompleteEmoji)
        val emojiNameText by bind<TextView>(R.id.itemAutocompleteEmojiName)
        val emojiKeywordText by bind<TextView>(R.id.itemAutocompleteEmojiSubname)
    }
}
