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

class TelegramFoldersActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        fun newIntent(context: Context): Intent =
            Intent(context, TelegramFoldersActivity::class.java)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.apply {
            title = "Chat Folders"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        addButton(container, "Load Folders") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatFolders(h)
            Toast.makeText(this, "Loading folders...", Toast.LENGTH_SHORT).show()
        }

        addButton(container, "Create Folder") {
            showCreateFolderDialog()
        }

        addButton(container, "Delete Folder") {
            val input = EditText(this).apply {
                hint = "Folder ID"
                inputType = android.text.InputType.TYPE_CLASS_NUMBER
            }
            AlertDialog.Builder(this)
                .setTitle("Delete Folder")
                .setView(input)
                .setPositiveButton("Delete") { _, _ ->
                    val folderId = input.text.toString().toIntOrNull() ?: 0
                    if (folderId > 0) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgDeleteChatFolder(h, folderId)
                        Toast.makeText(this, "Folder deleted", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Recommended Folders") {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatFolders(h)
            Toast.makeText(this, "Loading recommended folders...", Toast.LENGTH_SHORT).show()
        }

        val scrollView = android.widget.ScrollView(this).apply { addView(container) }
        views.telegramMsgRecycler.visibility = android.view.View.GONE
        val root = findViewById<android.view.ViewGroup>(android.R.id.content)
        root.addView(scrollView, android.view.ViewGroup.LayoutParams(
            android.view.ViewGroup.LayoutParams.MATCH_PARENT,
            android.view.ViewGroup.LayoutParams.MATCH_PARENT
        ))
    }

    private fun showCreateFolderDialog() {
        val layout = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
        }
        val nameInput = EditText(this).apply { hint = "Folder name" }
        val pinnedInput = EditText(this).apply { hint = "Pinned chat IDs (comma-separated)" }
        val includedInput = EditText(this).apply { hint = "Included chat IDs (comma-separated)" }
        val excludedInput = EditText(this).apply { hint = "Excluded chat IDs (comma-separated)" }

        layout.addView(nameInput)
        layout.addView(pinnedInput)
        layout.addView(includedInput)
        layout.addView(excludedInput)

        val scrollDialog = android.widget.ScrollView(this).apply { addView(layout) }

        AlertDialog.Builder(this)
            .setTitle("Create Chat Folder")
            .setView(scrollDialog)
            .setPositiveButton("Create") { _, _ ->
                val name = nameInput.text.toString().trim()
                if (name.isNotBlank()) {
                    val pinned = parseIds(pinnedInput.text.toString())
                    val included = parseIds(includedInput.text.toString())
                    val excluded = parseIds(excludedInput.text.toString())
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgSetChatFolder(h, 0, name, pinned, included, excluded)
                    Toast.makeText(this, "Folder created: $name", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun parseIds(text: String): LongArray {
        return text.split(",").mapNotNull { it.trim().toLongOrNull() }.toLongArray()
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
