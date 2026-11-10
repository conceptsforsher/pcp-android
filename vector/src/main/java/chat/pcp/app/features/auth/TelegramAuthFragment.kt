package chat.pcp.app.features.auth

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.EditorInfo
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.databinding.FragmentTelegramAuthBinding
import timber.log.Timber

class TelegramAuthFragment : Fragment() {

    private var _binding: FragmentTelegramAuthBinding? = null
    private val views get() = _binding!!

    private var nativeHandle: Long = 0
    private var currentStep: AuthStep = AuthStep.PHONE

    enum class AuthStep { PHONE, CODE, PASSWORD, READY }

    interface AuthCallback {
        fun onAuthReady(userId: String)
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentTelegramAuthBinding.inflate(inflater, container, false)
        return views.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupListeners()
    }

    fun bind(handle: Long) {
        nativeHandle = handle
        PcpNative.tgAuthListener = authListener
        showPhoneStep()
    }

    private val authListener = object : chat.pcp.app.native.TgAuthListener {
        override fun onAuthStateChanged(type: String, stateJson: String) {
            Timber.d("Auth state: %s", type)
            activity?.runOnUiThread {
                showLoading(false)
                when (type) {
                    "authorizationStateWaitPhoneNumber" -> showPhoneStep()
                    "authorizationStateWaitCode" -> showCodeStep("Code sent")
                    "authorizationStateWaitPassword" -> showPasswordStep()
                    "authorizationStateReady" -> {
                        currentStep = AuthStep.READY
                        showStatus(getString(R.string.tg_auth_status_ready))
                        views.tgAuthButton.isEnabled = false
                        views.tgAuthInputGroup.isVisible = false
                        val userId = PcpNative.tgGetUserId(nativeHandle)
                        if (userId.isNotEmpty()) {
                            (activity as? AuthCallback)?.onAuthReady(userId)
                        }
                    }
                    "authorizationStateClosed" -> showError(getString(R.string.tg_auth_status_closed))
                    "authorizationStateLoggingOut" -> showStatus(getString(R.string.tg_auth_status_logging_out))
                    else -> {
                        if (type.contains("Error") || type.contains("error")) {
                            showError(type)
                        }
                    }
                }
            }
        }

        override fun onConnectionStateChanged(state: String) {
            activity?.runOnUiThread {
                showStatus(state)
            }
        }
    }

    private fun setupListeners() {
        views.tgAuthPhoneInput.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_NEXT) { onButtonClicked(); true } else false
        }
        views.tgAuthCodeInput.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_NEXT || actionId == EditorInfo.IME_ACTION_DONE) { onButtonClicked(); true } else false
        }
        views.tgAuthPasswordInput.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_DONE) { onButtonClicked(); true } else false
        }
        views.tgAuthButton.setOnClickListener { onButtonClicked() }
    }

    private fun onButtonClicked() {
        when (currentStep) {
            AuthStep.PHONE -> {
                val phone = views.tgAuthPhoneInput.text?.toString()?.trim() ?: ""
                if (phone.isBlank()) { showError(getString(R.string.tg_auth_error_phone_empty)); return }
                showLoading(true)
                PcpNative.tgSendPhone(nativeHandle, phone)
            }
            AuthStep.CODE -> {
                val code = views.tgAuthCodeInput.text?.toString()?.trim() ?: ""
                if (code.isBlank()) { showError(getString(R.string.tg_auth_error_code_empty)); return }
                showLoading(true)
                PcpNative.tgSendCode(nativeHandle, code)
            }
            AuthStep.PASSWORD -> {
                val password = views.tgAuthPasswordInput.text?.toString() ?: ""
                if (password.isBlank()) { showError(getString(R.string.tg_auth_error_password_empty)); return }
                showLoading(true)
                PcpNative.tgSendPassword(nativeHandle, password)
            }
            AuthStep.READY -> {}
        }
    }

    // --- UI helpers ---

    private fun showPhoneStep() {
        currentStep = AuthStep.PHONE
        views.tgAuthHint.text = getString(R.string.tg_auth_phone_hint)
        views.tgAuthButton.text = getString(R.string.tg_auth_button_next)
        views.tgAuthPhoneLayout.isVisible = true
        views.tgAuthCodeLayout.isVisible = false
        views.tgAuthPasswordLayout.isVisible = false
        hideError(); hideStatus()
    }

    private fun showCodeStep(hint: String) {
        currentStep = AuthStep.CODE
        views.tgAuthHint.text = hint
        views.tgAuthButton.text = getString(R.string.tg_auth_button_verify)
        views.tgAuthPhoneLayout.isVisible = false
        views.tgAuthCodeLayout.isVisible = true
        views.tgAuthPasswordLayout.isVisible = false
        hideError(); hideStatus()
    }

    private fun showPasswordStep() {
        currentStep = AuthStep.PASSWORD
        views.tgAuthHint.text = getString(R.string.tg_auth_password_hint)
        views.tgAuthButton.text = getString(R.string.tg_auth_button_verify)
        views.tgAuthPhoneLayout.isVisible = false
        views.tgAuthCodeLayout.isVisible = false
        views.tgAuthPasswordLayout.isVisible = true
        hideError(); hideStatus()
    }

    fun showError(message: String) {
        views.tgAuthError.isVisible = true
        views.tgAuthError.text = message
        showLoading(false)
    }

    private fun hideError() { views.tgAuthError.isVisible = false }

    private fun showStatus(text: String) {
        views.tgAuthStatus.isVisible = true
        views.tgAuthStatus.text = text
    }

    private fun hideStatus() { views.tgAuthStatus.isVisible = false }

    private fun showLoading(loading: Boolean) {
        views.tgAuthButton.isEnabled = !loading
        if (loading) showStatus(getString(R.string.tg_auth_status_connecting))
    }

    override fun onDestroyView() {
        super.onDestroyView()
        PcpNative.tgAuthListener = null
        _binding = null
    }
}
