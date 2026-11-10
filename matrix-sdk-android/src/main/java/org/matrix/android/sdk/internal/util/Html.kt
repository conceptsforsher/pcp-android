package org.matrix.android.sdk.internal.util

import chat.pcp.app.native.PcpNative

internal fun String.unescapeHtml(): String = PcpNative.nativeUnescapeHtml(this)
