package chat.pcp.app.features.lemmy

import android.content.Context
import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import androidx.lifecycle.lifecycleScope
import chat.pcp.app.lemmy.LemmySessionHolder
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.databinding.ActivityLemmyAuthBinding
import im.vector.app.features.home.HomeActivity
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import timber.log.Timber
import javax.inject.Inject

@AndroidEntryPoint
class LemmyAuthActivity : AppCompatActivity() {

    @Inject lateinit var lemmySessionHolder: LemmySessionHolder

    private lateinit var binding: ActivityLemmyAuthBinding
    private var fromSplash: Boolean = false

    companion object {
        private const val EXTRA_FROM_SPLASH = "from_splash"

        fun newIntent(context: Context, fromSplash: Boolean = false): Intent {
            return Intent(context, LemmyAuthActivity::class.java).apply {
                putExtra(EXTRA_FROM_SPLASH, fromSplash)
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLemmyAuthBinding.inflate(layoutInflater)
        setContentView(binding.root)

        fromSplash = intent.getBooleanExtra(EXTRA_FROM_SPLASH, false)

        binding.lemmyLoginButton.setOnClickListener { onLoginClick() }
    }

    private fun onLoginClick() {
        val instance = binding.lemmyInstanceInput.text.toString().trim()
        val username = binding.lemmyUsernameInput.text.toString().trim()
        val password = binding.lemmyPasswordInput.text.toString()

        if (instance.isEmpty() || username.isEmpty() || password.isEmpty()) {
            showError("Please fill in all fields")
            return
        }

        showLoading(true)

        lifecycleScope.launch {
            try {
                val success = withContext(Dispatchers.IO) {
                    val session = lemmySessionHolder.getOrCreateSession(instance)
                    session.login(username, password)
                }

                if (success) {
                    onAuthSuccess()
                } else {
                    showError("Login failed. Check your credentials.")
                }
            } catch (e: Exception) {
                Timber.e(e, "Lemmy auth failed")
                showError("Error: ${e.message}")
            } finally {
                showLoading(false)
            }
        }
    }

    private fun showLoading(loading: Boolean) {
        binding.lemmyAuthProgress.isVisible = loading
        binding.lemmyLoginButton.isEnabled = !loading
        binding.lemmyInstanceInput.isEnabled = !loading
        binding.lemmyUsernameInput.isEnabled = !loading
        binding.lemmyPasswordInput.isEnabled = !loading
    }

    private fun showError(message: String) {
        binding.lemmyAuthError.apply {
            text = message
            isVisible = true
        }
    }

    private fun onAuthSuccess() {
        if (fromSplash) {
            startActivity(HomeActivity.newIntent(this, firstStartMainActivity = true))
        }
        finish()
    }
}
