/*
 * Copyright 2019-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.error

import android.content.ActivityNotFoundException
import chat.pcp.app.core.resources.StringProvider
import chat.pcp.app.features.call.dialpad.DialPadLookup
import chat.pcp.app.features.roomprofile.polls.RoomPollsLoadingError
import chat.pcp.app.features.voice.VoiceFailure
import chat.pcp.app.features.voicebroadcast.VoiceBroadcastFailure
import chat.pcp.app.features.voicebroadcast.VoiceBroadcastFailure.RecordingError
import chat.pcp.app.native.PcpNative
import chat.pcp.lib.strings.CommonPlurals
import chat.pcp.lib.strings.CommonStrings
import org.matrix.android.sdk.api.failure.Failure
import org.matrix.android.sdk.api.failure.MatrixError
import org.matrix.android.sdk.api.failure.MatrixIdFailure
import org.matrix.android.sdk.api.failure.isLimitExceededError
import org.matrix.android.sdk.api.session.identity.IdentityServiceError
import java.net.HttpURLConnection
import java.net.SocketTimeoutException
import javax.inject.Inject
import javax.net.ssl.SSLException
import javax.net.ssl.SSLPeerUnverifiedException

interface ErrorFormatter {
    fun toHumanReadable(throwable: Throwable?): String
}

class DefaultErrorFormatter @Inject constructor(
        private val stringProvider: StringProvider
) : ErrorFormatter {

    override fun toHumanReadable(throwable: Throwable?): String {
        return when (throwable) {
            null -> null
            is IdentityServiceError -> identityServerError(throwable)
            is Failure.NetworkConnection -> {
                val type = classifyNetwork(throwable.ioException)
                resolve(type)
            }
            is Failure.ServerError -> {
                if (throwable.isLimitExceededError()) {
                    limitExceededError(throwable.error)
                } else {
                    val type = PcpNative.nativeClassifyError(
                        throwable.error.code,
                        HttpURLConnection.HTTP_INTERNAL_ERROR,
                        throwable.error.message,
                        false, false
                    )
                    resolve(type) ?: (throwable.error.message.takeIf { it.isNotEmpty() }
                        ?: throwable.error.code.takeIf { it.isNotEmpty() })
                }
            }
            is Failure.OtherServerError -> {
                val isNotFound = throwable.httpCode == HttpURLConnection.HTTP_NOT_FOUND
                val isUnauth = throwable.httpCode == HttpURLConnection.HTTP_UNAUTHORIZED
                when {
                    isNotFound -> stringProvider.getString(CommonStrings.login_error_no_homeserver_found)
                    isUnauth -> stringProvider.getString(CommonStrings.error_unauthorized)
                    else -> throwable.localizedMessage
                }
            }
            is DialPadLookup.Failure.NumberIsYours ->
                stringProvider.getString(CommonStrings.cannot_call_yourself)
            is DialPadLookup.Failure.NoResult ->
                stringProvider.getString(CommonStrings.call_dial_pad_lookup_error)
            is MatrixIdFailure.InvalidMatrixId ->
                stringProvider.getString(CommonStrings.login_signin_matrix_id_error_invalid_matrix_id)
            is VoiceFailure -> voiceMessageError(throwable)
            is VoiceBroadcastFailure -> voiceBroadcastMessageError(throwable)
            is RoomPollsLoadingError -> stringProvider.getString(CommonStrings.room_polls_loading_error)
            is ActivityNotFoundException ->
                stringProvider.getString(CommonStrings.error_no_external_application_found)
            else -> throwable.localizedMessage
        } ?: stringProvider.getString(CommonStrings.unknown_error)
    }

    private fun classifyNetwork(ioException: java.io.IOException?): String {
        return when (ioException) {
            is SocketTimeoutException -> "network_timeout"
            is SSLPeerUnverifiedException -> "ssl_peer_unverified"
            is SSLException -> "ssl_other"
            else -> "no_network"
        }
    }

    private fun resolve(type: String?): String? {
        return when (type) {
            "network_timeout" -> stringProvider.getString(CommonStrings.error_network_timeout)
            "ssl_peer_unverified" -> stringProvider.getString(CommonStrings.login_error_ssl_peer_unverified)
            "ssl_other" -> stringProvider.getString(CommonStrings.login_error_ssl_other)
            "no_network" -> stringProvider.getString(CommonStrings.error_no_network)
            "terms_not_accepted" -> stringProvider.getString(CommonStrings.error_terms_not_accepted)
            "invalid_password" -> stringProvider.getString(CommonStrings.auth_invalid_login_param)
            "user_in_use" -> stringProvider.getString(CommonStrings.login_signup_error_user_in_use)
            "bad_json" -> stringProvider.getString(CommonStrings.login_error_bad_json)
            "not_json" -> stringProvider.getString(CommonStrings.login_error_not_json)
            "threepid_denied" -> stringProvider.getString(CommonStrings.login_error_threepid_denied)
            "file_too_big" -> stringProvider.getString(CommonStrings.error_file_too_big_simple)
            "threepid_not_found" -> stringProvider.getString(CommonStrings.login_reset_password_error_not_found)
            "user_deactivated" -> stringProvider.getString(CommonStrings.auth_invalid_login_deactivated_account)
            "email_already_used" -> stringProvider.getString(CommonStrings.account_email_already_used_error)
            "phone_already_used" -> stringProvider.getString(CommonStrings.account_phone_number_already_used_error)
            "threepid_auth_failed" -> stringProvider.getString(CommonStrings.error_threepid_auth_failed)
            "room_access_unauthorized" -> stringProvider.getString(CommonStrings.room_error_access_unauthorized)
            "unverified_email" -> stringProvider.getString(CommonStrings.auth_reset_password_error_unverified)
            "homeserver_not_found" -> stringProvider.getString(CommonStrings.login_error_no_homeserver_found)
            "unauthorized" -> stringProvider.getString(CommonStrings.error_unauthorized)
            else -> null
        }
    }

    private fun voiceMessageError(throwable: VoiceFailure): String {
        return when (throwable) {
            is VoiceFailure.UnableToPlay -> stringProvider.getString(CommonStrings.error_voice_message_unable_to_play)
            is VoiceFailure.UnableToRecord -> stringProvider.getString(CommonStrings.error_voice_message_unable_to_record)
            is VoiceFailure.VoiceBroadcastInProgress -> stringProvider.getString(CommonStrings.error_voice_message_broadcast_in_progress)
        }
    }

    private fun voiceBroadcastMessageError(throwable: VoiceBroadcastFailure): String {
        return when (throwable) {
            RecordingError.BlockedBySomeoneElse -> stringProvider.getString(CommonStrings.error_voice_broadcast_blocked_by_someone_else_message)
            RecordingError.NoPermission -> stringProvider.getString(CommonStrings.error_voice_broadcast_permission_denied_message)
            RecordingError.UserAlreadyBroadcasting -> stringProvider.getString(CommonStrings.error_voice_broadcast_already_in_progress_message)
            is VoiceBroadcastFailure.ListeningError.UnableToPlay,
            is VoiceBroadcastFailure.ListeningError.PrepareMediaPlayerError -> stringProvider.getString(CommonStrings.error_voice_broadcast_unable_to_play)
            is VoiceBroadcastFailure.ListeningError.UnableToDecrypt -> stringProvider.getString(CommonStrings.error_voice_broadcast_unable_to_decrypt)
        }
    }

    private fun limitExceededError(error: MatrixError): String {
        val delay = error.retryAfterMillis
        return if (delay == null) {
            stringProvider.getString(CommonStrings.login_error_limit_exceeded)
        } else {
            val delaySeconds = delay.toInt() / 1000 + 1
            stringProvider.getQuantityString(CommonPlurals.login_error_limit_exceeded_retry_after, delaySeconds, delaySeconds)
        }
    }

    private fun identityServerError(identityServiceError: IdentityServiceError): String {
        return stringProvider.getString(
                when (identityServiceError) {
                    IdentityServiceError.OutdatedIdentityServer -> CommonStrings.identity_server_error_outdated_identity_server
                    IdentityServiceError.OutdatedHomeServer -> CommonStrings.identity_server_error_outdated_home_server
                    IdentityServiceError.NoIdentityServerConfigured -> CommonStrings.identity_server_error_no_identity_server_configured
                    IdentityServiceError.TermsNotSignedException -> CommonStrings.identity_server_error_terms_not_signed
                    IdentityServiceError.BulkLookupSha256NotSupported -> CommonStrings.identity_server_error_bulk_sha256_not_supported
                    IdentityServiceError.BindingError -> CommonStrings.identity_server_error_binding_error
                    IdentityServiceError.NoCurrentBindingError -> CommonStrings.identity_server_error_no_current_binding_error
                    IdentityServiceError.UserConsentNotProvided -> CommonStrings.identity_server_user_consent_not_provided
                }
        )
    }
}
