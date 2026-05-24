package chat.progressive.app.features.delta

import android.content.Context
import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import androidx.lifecycle.lifecycleScope
import chat.progressive.app.delta.DeltaSessionHolder
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.databinding.ActivityDeltaAuthBinding
import im.vector.app.features.home.HomeActivity
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import timber.log.Timber
import javax.inject.Inject

@AndroidEntryPoint
class DeltaAuthActivity : AppCompatActivity() {

    @Inject lateinit var deltaSessionHolder: DeltaSessionHolder

    private lateinit var binding: ActivityDeltaAuthBinding
    private var fromSplash: Boolean = false

    companion object {
        private const val EXTRA_FROM_SPLASH = "from_splash"

        fun newIntent(context: Context, fromSplash: Boolean = false): Intent {
            return Intent(context, DeltaAuthActivity::class.java).apply {
                putExtra(EXTRA_FROM_SPLASH, fromSplash)
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityDeltaAuthBinding.inflate(layoutInflater)
        setContentView(binding.root)

        fromSplash = intent.getBooleanExtra(EXTRA_FROM_SPLASH, false)

        binding.deltaLoginButton.setOnClickListener { onLoginClick() }

        val session = deltaSessionHolder.getSession()
        if (session != null && session.isConfigured()) {
            onAuthSuccess()
        }
    }

    private fun onLoginClick() {
        val email = binding.deltaEmailInput.text.toString().trim()
        val password = binding.deltaPasswordInput.text.toString()

        if (email.isEmpty() || password.isEmpty()) {
            showError("Please enter email and password")
            return
        }

        showLoading(true)

        lifecycleScope.launch {
            try {
                val success = withContext(Dispatchers.IO) {
                    val session = deltaSessionHolder.getOrCreateSession() ?: return@withContext false
                    session.configure(email, password)
                    session.open()
                    session.isConfigured()
                }

                if (success) {
                    onAuthSuccess()
                } else {
                    showError("Failed to configure DeltaChat. Check your credentials.")
                }
            } catch (e: Exception) {
                Timber.e(e, "DeltaChat auth failed")
                showError("Error: ${e.message}")
            } finally {
                showLoading(false)
            }
        }
    }

    private fun showLoading(loading: Boolean) {
        binding.deltaAuthProgress.isVisible = loading
        binding.deltaLoginButton.isEnabled = !loading
        binding.deltaEmailInput.isEnabled = !loading
        binding.deltaPasswordInput.isEnabled = !loading
    }

    private fun showError(message: String) {
        binding.deltaAuthError.apply {
            text = message
            isVisible = true
        }
    }

    private fun onAuthSuccess() {
        if (fromSplash) {
            startActivity(HomeActivity.newIntent(this, firstStartMainActivity = true))
        } else {
            startActivity(DeltaChatListActivity.newIntent(this))
        }
        finish()
    }
}
