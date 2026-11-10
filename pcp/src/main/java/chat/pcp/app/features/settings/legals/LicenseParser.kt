/*
 * Copyright 2024-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.settings.legals

/**
 * Parsed license entry from open_source_licenses.html.
 */
data class LicenseEntry(
    val name: String,
    val copyright: String,
    val licenseText: String,
)

/**
 * Parses the open_source_licenses.html content into structured data.
 * HTML structure:
 *   <div><p>App Name</p><p>Third Party Licenses</p></div>
 *   <ul><li><b>libname</b><br/>Copyright...</li></ul>
 *   <pre>license text...</pre>
 *   ...repeat...
 *   <h3>Apache License...</h3>
 *   <ul><li>...</li><li>...</li></ul>
 *   <pre>Apache text...</pre>
 */
object LicenseParser {

    private val LICENSE_BLOCK = Regex(
        """<ul>\s*<li>\s*<b>(.+?)</b>\s*<br/?>\s*(.*?)\s*</li>\s*</ul>\s*<pre>(.+?)</pre>""",
        RegexOption.DOT_MATCHES_ALL
    )

    private val APACHE_ITEM = Regex(
        """<li>\s*<b>(.+?)</b>\s*<br/?>\s*(.*?)\s*</li>""",
        RegexOption.DOT_MATCHES_ALL
    )

    private val APACHE_BLOCK = Regex(
        """<h3>\s*Apache License.+?</h3>\s*(<ul>.+?</ul>)\s*<pre>(.+?)</pre>""",
        RegexOption.DOT_MATCHES_ALL
    )

    fun parse(html: String): List<LicenseEntry> {
        val entries = mutableListOf<LicenseEntry>()

        // Parse individual license blocks (BSD, MIT, etc.)
        LICENSE_BLOCK.findAll(html).forEach { match ->
            val name = match.groupValues[1].trim()
            val copyright = match.groupValues[2].trim()
            val text = unescapeHtml(match.groupValues[3].trim())
            entries.add(LicenseEntry(name, copyright, text))
        }

        // Parse Apache-licensed items as a group
        APACHE_BLOCK.find(html)?.let { match ->
            val itemsBlock = match.groupValues[1]
            val apacheText = unescapeHtml(match.groupValues[2].trim())

            APACHE_ITEM.findAll(itemsBlock).forEach { item ->
                val name = item.groupValues[1].trim()
                val copyright = item.groupValues[2].trim()
                entries.add(LicenseEntry(name, copyright, apacheText))
            }
        }

        return entries.distinctBy { it.name }
    }

    private fun unescapeHtml(text: String): String = text
        .replace("&amp;", "&")
        .replace("&lt;", "<")
        .replace("&gt;", ">")
        .replace("&quot;", "\"")
        .replace("&#39;", "'")
        .trim()
}
