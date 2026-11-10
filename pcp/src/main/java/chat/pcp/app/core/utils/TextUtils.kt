/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.utils

import android.annotation.SuppressLint
import android.content.Context
import android.os.Build
import android.text.format.Formatter
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.lib.strings.CommonStrings
import org.threeten.bp.Duration

object TextUtils {

    private const val MINUTES_PER_HOUR = 60
    private const val SECONDS_PER_MINUTE = 60

    fun formatCountToShortDecimal(value: Int): String {
        return chat.pcp.app.native.PcpNative.nativeFormatCountToShortDecimal(value)
    }

    /**
     * Since Android O, the system considers that 1ko = 1000 bytes instead of 1024 bytes. We want to avoid that for the moment.
     */
    fun formatFileSize(context: Context, sizeBytes: Long, useShortFormat: Boolean = false): String {
        val normalizedSize = if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.N) {
            sizeBytes
        } else {
            // First convert the size
            when {
                sizeBytes < 1024 -> sizeBytes
                sizeBytes < 1024 * 1024 -> sizeBytes * 1000 / 1024
                sizeBytes < 1024 * 1024 * 1024 -> sizeBytes * 1000 / 1024 * 1000 / 1024
                else -> sizeBytes * 1000 / 1024 * 1000 / 1024 * 1000 / 1024
            }
        }

        return if (useShortFormat) {
            Formatter.formatShortFileSize(context, normalizedSize)
        } else {
            Formatter.formatFileSize(context, normalizedSize)
        }
    }

    @SuppressLint("DefaultLocale")
    fun formatDuration(duration: Duration): String {
        val hours = getHours(duration)
        val minutes = getMinutes(duration)
        val seconds = getSeconds(duration)
        return if (hours > 0) {
            String.format("%d:%02d:%02d", hours, minutes, seconds)
        } else {
            String.format("%02d:%02d", minutes, seconds)
        }
    }

    fun formatDurationWithUnits(context: Context, duration: Duration, appendSeconds: Boolean = true): String {
        return formatDurationWithUnits(duration, context::getString, appendSeconds)
    }

    fun formatDurationWithUnits(stringProvider: StringProvider, duration: Duration, appendSeconds: Boolean = true): String {
        return formatDurationWithUnits(duration, stringProvider::getString, appendSeconds)
    }

    /**
     * We don't always have Context to get strings or we want to use StringProvider instead.
     * So we can pass the getString function either from Context or the StringProvider.
     * @param duration duration to be formatted
     * @param getString getString method from Context or StringProvider
     * @param appendSeconds if false than formatter will not append seconds
     * @return formatted duration with a localized form like "10h 30min 5sec"
     */
    private fun formatDurationWithUnits(duration: Duration, getString: ((Int) -> String), appendSeconds: Boolean = true): String {
        val hours = getHours(duration)
        val minutes = getMinutes(duration)
        val seconds = getSeconds(duration)
        val builder = StringBuilder()
        when {
            hours > 0 -> {
                appendHours(getString, builder, hours)
                if (minutes > 0) {
                    builder.append(" ")
                    appendMinutes(getString, builder, minutes)
                }
                if (appendSeconds && seconds > 0) {
                    builder.append(" ")
                    appendSeconds(getString, builder, seconds)
                }
            }
            minutes > 0 -> {
                appendMinutes(getString, builder, minutes)
                if (appendSeconds && seconds > 0) {
                    builder.append(" ")
                    appendSeconds(getString, builder, seconds)
                }
            }
            else -> {
                appendSeconds(getString, builder, seconds)
            }
        }
        return builder.toString()
    }

    private fun appendHours(getString: ((Int) -> String), builder: StringBuilder, hours: Int) {
        builder.append(hours)
        builder.append(getString(CommonStrings.time_unit_hour_short))
    }

    private fun appendMinutes(getString: ((Int) -> String), builder: StringBuilder, minutes: Int) {
        builder.append(minutes)
        builder.append(getString(CommonStrings.time_unit_minute_short))
    }

    private fun appendSeconds(getString: ((Int) -> String), builder: StringBuilder, seconds: Int) {
        builder.append(seconds)
        builder.append(getString(CommonStrings.time_unit_second_short))
    }

    private fun getHours(duration: Duration): Int = duration.toHours().toInt()
    private fun getMinutes(duration: Duration): Int = duration.toMinutes().toInt() % MINUTES_PER_HOUR
    private fun getSeconds(duration: Duration): Int = (duration.seconds % SECONDS_PER_MINUTE).toInt()
}
