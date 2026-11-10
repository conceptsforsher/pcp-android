package chat.pcp.app.features.lemmy.home

import android.text.Html
import android.text.Spanned
import android.text.SpannedString
import android.os.Build
import chat.pcp.app.native.PcpNative
import timber.log.Timber

object LemmyMarkdown {

    fun render(markdown: String, enableTables: Boolean = false): Spanned {
        return try {
            val html = PcpNative.nativeMarkdownToHtml(markdown, enableTables)
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.N) {
                Html.fromHtml(html, Html.FROM_HTML_MODE_LEGACY)
            } else {
                @Suppress("DEPRECATION")
                Html.fromHtml(html)
            }
        } catch (e: Exception) {
            Timber.w(e, "Markdown render failed, falling back to plain text")
            SpannedString(markdown)
        }
    }
}
