package org.matrix.android.sdk.api.util

import chat.pcp.app.native.PcpNative
import org.json.JSONArray

object StringOrderUtils {

    val DEFAULT_ALPHABET = buildString {
        for (i in 0x20..0x7E) append(Char(i))
    }.toCharArray()

    @Suppress("UNUSED_PARAMETER")
    fun average(left: String, right: String, alphabet: CharArray = DEFAULT_ALPHABET): String? {
        val result = PcpNative.nativeStringAverage(left, right)
        return result.takeIf { it != left && it != right }
    }

    @Suppress("UNUSED_PARAMETER")
    fun midPoints(left: String, right: String, count: Int, alphabet: CharArray = DEFAULT_ALPHABET): List<String>? {
        if (left == right) return null
        val a = if (left > right) right else left
        val b = if (left > right) left else right
        val json = PcpNative.nativeStringMidPoints(a, b, count)
        val arr = JSONArray(json)
        val result = (0 until arr.length()).map { arr.getString(it) }
        return result.takeIf { it.isNotEmpty() && it.last() < b && it.first() > a }
    }
}
