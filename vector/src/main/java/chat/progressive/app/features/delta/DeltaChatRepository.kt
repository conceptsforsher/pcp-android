package chat.progressive.app.features.delta

import android.content.Context
import chat.progressive.app.delta.DeltaSessionHolder
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class DeltaChatRepository @Inject constructor(
    private val context: Context,
    private val deltaSessionHolder: DeltaSessionHolder
) {
    private val _isLoggedIn = MutableStateFlow(false)
    val isLoggedIn: StateFlow<Boolean> = _isLoggedIn

    fun init() {
        val session = deltaSessionHolder.getOrCreateSession()
        _isLoggedIn.value = session?.isConfigured() == true
    }

    fun refresh() {
        val session = deltaSessionHolder.getSession()
        _isLoggedIn.value = session?.isConfigured() == true
    }
}
