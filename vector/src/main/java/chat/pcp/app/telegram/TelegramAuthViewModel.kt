package chat.pcp.app.telegram

import android.content.Context
import android.content.Intent
import android.net.Uri
import androidx.activity.result.ActivityResultLauncher
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch

class TelegramAuthViewModel(
    private val holder: TelegramSessionHolder
) : ViewModel() {

    private val _state = MutableStateFlow<TelegramAuthUiState>(TelegramAuthUiState.Loading)
    val state: StateFlow<TelegramAuthUiState> = _state.asStateFlow()

    private var session: TelegramSession? = null

    fun initialize() {
        viewModelScope.launch {
            val s = holder.getOrCreateSession()
            if (s == null) {
                _state.value = TelegramAuthUiState.Error("TDLib not available")
                return@launch
            }
            session = s

            val tdClient = holder.getTdClient() ?: run {
                _state.value = TelegramAuthUiState.Error("TDLib client not created")
                return@launch
            }

            tdClient.addListener(object : TelegramUpdateListener {
                override fun onConnectionStateChanged(state: ConnectionState) {
                    if (state == ConnectionState.CONNECTED) {
                        _state.value = TelegramAuthUiState.LoggedIn
                    }
                }
            })

            viewModelScope.launch {
                tdClient.authState.collect { authState ->
                    _state.value = when (authState.state) {
                        AuthStep.WAITING_PHONE_NUMBER -> TelegramAuthUiState.WaitingPhoneNumber
                        AuthStep.WAITING_CODE -> TelegramAuthUiState.WaitingCode(authState.codeInfo ?: "")
                        AuthStep.WAITING_PASSWORD -> TelegramAuthUiState.WaitingPassword
                        AuthStep.READY -> TelegramAuthUiState.LoggedIn
                        AuthStep.CLOSED -> TelegramAuthUiState.Closed
                        AuthStep.ERROR -> TelegramAuthUiState.Error(authState.hint ?: "Unknown error")
                    }
                }
            }

            s.open()
        }
    }

    fun submitPhoneNumber(phone: String) {
        holder.getTdClient()?.sendPhoneNumber(phone)
    }

    fun submitCode(code: String) {
        holder.getTdClient()?.sendCode(code)
    }

    fun submitPassword(password: String) {
        holder.getTdClient()?.sendPassword(password)
    }

    fun openTelegramRegistrationUrl(context: Context, launcher: ActivityResultLauncher<Intent>) {
        val intent = Intent(Intent.ACTION_VIEW, Uri.parse("https://my.telegram.org/apps"))
        launcher.launch(intent)
    }

    override fun onCleared() {
        super.onCleared()
        holder.close()
    }
}

sealed class TelegramAuthUiState {
    data object Loading : TelegramAuthUiState()
    data object WaitingPhoneNumber : TelegramAuthUiState()
    data class WaitingCode(val codeInfo: String) : TelegramAuthUiState()
    data object WaitingPassword : TelegramAuthUiState()
    data object LoggedIn : TelegramAuthUiState()
    data object Closed : TelegramAuthUiState()
    data class Error(val message: String) : TelegramAuthUiState()
}
