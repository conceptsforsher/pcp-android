/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.core.linkify

/**
 * Better support for geo URI.
 */
object PcpAutoLink {

    // geo:
    private const val LAT_OR_LONG_OR_ALT_NUMBER = "-?\\d+(?:\\.\\d+)?"
    private const val COORDINATE_SYSTEM = ";crs=[\\w-]+"

    val GEO_URI: Regex = Regex(
            "(?:geo:)?" +
                    "(" + LAT_OR_LONG_OR_ALT_NUMBER + ")" +
                    "," +
                    "(" + LAT_OR_LONG_OR_ALT_NUMBER + ")" +
                    "(?:" + "," + LAT_OR_LONG_OR_ALT_NUMBER + ")?" + // altitude
                    "(?:" + COORDINATE_SYSTEM + ")?" +
                    "(?:" + ";u=\\d+(?:\\.\\d+)?" + ")?" + // uncertainty in meters
                    "(?:" +
                    ";[\\w-]+=(?:[\\w-_.!~*'()]|%[\\da-f][\\da-f])+" + // dafuk
                    ")*", RegexOption.IGNORE_CASE
    )
}
