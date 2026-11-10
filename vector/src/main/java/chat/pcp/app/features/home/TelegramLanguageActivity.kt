package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.Toast
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding

class TelegramLanguageActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        fun newIntent(context: Context): Intent =
            Intent(context, TelegramLanguageActivity::class.java)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.apply {
            title = "Language"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        val languages = listOf(
            "English" to "en",
            "Русский" to "ru",
            "Español" to "es",
            "Deutsch" to "de",
            "Français" to "fr",
            "Italiano" to "it",
            "Português" to "pt",
            "العربية" to "ar",
            "中文" to "zh",
            "日本語" to "ja",
            "한국어" to "ko",
            "Türkçe" to "tr",
            "Українська" to "uk",
            "Bahasa Indonesia" to "id"
        )

        languages.forEach { (name, code) ->
            addButton(container, name) {
                Toast.makeText(this, "Language set: $name ($code)", Toast.LENGTH_SHORT).show()
            }
        }

        val scrollView = android.widget.ScrollView(this).apply { addView(container) }
        views.telegramMsgRecycler.visibility = android.view.View.GONE
        val root = findViewById<android.view.ViewGroup>(android.R.id.content)
        root.addView(scrollView, android.view.ViewGroup.LayoutParams(
            android.view.ViewGroup.LayoutParams.MATCH_PARENT,
            android.view.ViewGroup.LayoutParams.MATCH_PARENT
        ))
    }

    private fun addButton(container: android.widget.LinearLayout, text: String, onClick: () -> Unit) {
        val btn = android.widget.Button(this).apply {
            this.text = text
            setTextColor(0xFF000000.toInt())
            setOnClickListener { onClick() }
        }
        container.addView(btn)
    }
}
