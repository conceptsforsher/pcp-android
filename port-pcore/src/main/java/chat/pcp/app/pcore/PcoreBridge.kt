package chat.pcp.app.pcore

/**
 * The single Kotlin seam to the native pcp-core. Every function is a
 * suspend call dispatched on the core's own thread; nothing else in the app
 * talks to pcore directly (see docs/PCORE_PORT_PLAN.md).
 */
object PcoreBridge {
/*
 * Phase-0.5 surface scan (docs/PCORE_PHASE05_TOP5.md): the top-5 room-seam
 * files reference **93 distinct org.matrix.android.sdk classes**. Bulk of
 * them are DATA MODELS (Message*Content, RoomSummary, Event, Membership,
 * ChangeMembershipState…) — these will be mirrored as plain Kotlin data
 * classes inside this module (bridge DTOs), generated mechanically from the
 * SDK sources in Phase 2 to avoid 90+ hand-written files.
 *
 * Behavioural seam shrinks to a handful of suspend calls:
 *   session scope : login/tokenRestore/syncOnce/observeState
 *   room scope    : timeline(room,batch) / sendText / redact / react /
 *                   setReadMarkers / membership ops
 *   crypto scope  : verify/backup (Phase 5)
 *   media scope   : upload/download (Phase 6)
 */

    init { System.loadLibrary("pcore_jni") }

    /** Phase 1 exit check. */
    external fun ping(): String

    // Phase 2+ stubs — implemented incrementally per plan phase.
    // external suspend fun login(homeserver: String, user: String, pass: String): String
    // external fun sync(token: String, since: String?): ByteArray
}
