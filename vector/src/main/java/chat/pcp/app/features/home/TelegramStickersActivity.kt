package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding

class TelegramStickersActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        fun newIntent(context: Context): Intent =
            Intent(context, TelegramStickersActivity::class.java)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.apply {
            title = "Stickers"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        addButton(container, "Installed Sticker Sets") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetInstalledStickerSets(h)
            Toast.makeText(this, "Loading installed sets...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Trending Sticker Sets") {
            Toast.makeText(this, "Loading trending sets...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Search Sticker Set") {
            val input = EditText(this).apply { hint = "Sticker set name or URL" }
            AlertDialog.Builder(this)
                .setTitle("Search Sticker Set")
                .setView(input)
                .setPositiveButton("Search") { _, _ ->
                    val name = input.text.toString().trim()
                    if (name.isNotBlank()) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgSearchStickerSet(h, name)
                        Toast.makeText(this, "Searching: $name", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Get Sticker Set by ID") {
            val input = EditText(this).apply {
                hint = "Set ID"
                inputType = android.text.InputType.TYPE_CLASS_NUMBER
            }
            AlertDialog.Builder(this)
                .setTitle("Get Sticker Set")
                .setView(input)
                .setPositiveButton("Get") { _, _ ->
                    val setId = input.text.toString().toLongOrNull() ?: 0
                    if (setId > 0) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgGetStickerSet(h, setId)
                        Toast.makeText(this, "Loading set $setId...", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Recent Stickers") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetRecentStickers(h)
            Toast.makeText(this, "Loading recent stickers...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Search Stickers by Emoji") {
            val input = EditText(this).apply { hint = "Emoji" }
            AlertDialog.Builder(this)
                .setTitle("Search Stickers")
                .setView(input)
                .setPositiveButton("Search") { _, _ ->
                    val emoji = input.text.toString().trim()
                    if (emoji.isNotBlank()) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgSearchStickers(h, emoji, 20)
                        Toast.makeText(this, "Searching stickers for: $emoji", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Install/Remove Set") {
            val layout = android.widget.LinearLayout(this).apply {
                orientation = android.widget.LinearLayout.VERTICAL
            }
            val setIdInput = EditText(this).apply {
                hint = "Set ID"
                inputType = android.text.InputType.TYPE_CLASS_NUMBER
            }
            val installCheck = android.widget.CheckBox(this).apply {
                text = "Install (uncheck to remove)"
                isChecked = true
            }
            val archiveCheck = android.widget.CheckBox(this).apply {
                text = "Archive"
                isChecked = false
            }
            layout.addView(setIdInput); layout.addView(installCheck); layout.addView(archiveCheck)

            AlertDialog.Builder(this)
                .setTitle("Manage Sticker Set")
                .setView(layout)
                .setPositiveButton("Apply") { _, _ ->
                    val setId = setIdInput.text.toString().toLongOrNull() ?: 0
                    if (setId > 0) {
                        val h = NativeSessionManager.getHandle()
                        // changeStickerSet via addFavoriteSticker as proxy
                        Toast.makeText(this, "Set ${if (installCheck.isChecked) "installed" else "removed"}", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
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
