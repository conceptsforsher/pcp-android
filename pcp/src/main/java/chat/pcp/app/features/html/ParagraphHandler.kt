/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.html

import chat.pcp.app.core.utils.DimensionConverter
import io.noties.markwon.MarkwonVisitor
import io.noties.markwon.SpannableBuilder
import io.noties.markwon.html.HtmlTag
import io.noties.markwon.html.MarkwonHtmlRenderer
import io.noties.markwon.html.TagHandler
import me.gujun.android.span.style.VerticalPaddingSpan

class ParagraphHandler(private val dimensionConverter: DimensionConverter) : TagHandler() {

    override fun supportedTags() = listOf("p")

    override fun handle(visitor: MarkwonVisitor, renderer: MarkwonHtmlRenderer, tag: HtmlTag) {
        if (tag.isBlock) {
            visitChildren(visitor, renderer, tag.asBlock)
        }
        SpannableBuilder.setSpans(
                visitor.builder(),
                VerticalPaddingSpan(dimensionConverter.dpToPx(4), dimensionConverter.dpToPx(4)),
                tag.start(),
                tag.end()
        )
    }
}
