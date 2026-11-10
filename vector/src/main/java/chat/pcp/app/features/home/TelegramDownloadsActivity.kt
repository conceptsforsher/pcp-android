package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.view.View
import android.os.Looper
import android.widget.Toast
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding

class TelegramDownloadsActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        fun newIntent(context: Context): Intent =
            Intent(context, TelegramDownloadsActivity::class.java)
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.apply {
            title = "Downloads"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.visibility = android.view.View.GONE

        val container = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        val label = android.widget.TextView(this).apply {
            text = "File Downloads"
            textSize = 18f
            setTextColor(0xFF000000.toInt())
            setPadding(0, 0, 0, 16)
        }
        container.addView(label)

        val statusLabel = android.widget.TextView(this).apply {
            text = "No active downloads"
            setTextColor(0xFF888888.toInt())
            setPadding(0, 0, 0, 16)
        }
        container.addView(statusLabel)

        addButton(container, "Download File") {
            val input = android.widget.EditText(this).apply {
                hint = "File ID"
                inputType = android.text.InputType.TYPE_CLASS_NUMBER
            }
            androidx.appcompat.app.AlertDialog.Builder(this)
                .setTitle("Download File")
                .setView(input)
                .setPositiveButton("Download") { _, _ ->
                    val fileId = input.text.toString().toIntOrNull() ?: 0
                    if (fileId > 0) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgDownloadFile(h, fileId, 1, 0, 0, false)
                        statusLabel.text = "Downloading file $fileId..."
                        Toast.makeText(this, "Download started", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Cancel Download") {
            val input = android.widget.EditText(this).apply {
                hint = "File ID"
                inputType = android.text.InputType.TYPE_CLASS_NUMBER
            }
            androidx.appcompat.app.AlertDialog.Builder(this)
                .setTitle("Cancel Download")
                .setView(input)
                .setPositiveButton("Cancel") { _, _ ->
                    val fileId = input.text.toString().toIntOrNull() ?: 0
                    if (fileId > 0) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgCancelDownload(h, fileId)
                        statusLabel.text = "No active downloads"
                        Toast.makeText(this, "Download cancelled", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Get File Info") {
            val input = android.widget.EditText(this).apply {
                hint = "File ID"
                inputType = android.text.InputType.TYPE_CLASS_NUMBER
            }
            androidx.appcompat.app.AlertDialog.Builder(this)
                .setTitle("Get File Info")
                .setView(input)
                .setPositiveButton("Get") { _, _ ->
                    val fileId = input.text.toString().toIntOrNull() ?: 0
                    if (fileId > 0) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgGetFile(h, fileId)
                        Toast.makeText(this, "Loading file info...", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
        }

        addButton(container, "Delete File") {
            val input = android.widget.EditText(this).apply {
                hint = "File ID"
                inputType = android.text.InputType.TYPE_CLASS_NUMBER
            }
            androidx.appcompat.app.AlertDialog.Builder(this)
                .setTitle("Delete File")
                .setView(input)
                .setPositiveButton("Delete") { _, _ ->
                    val fileId = input.text.toString().toIntOrNull() ?: 0
                    if (fileId > 0) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgDeleteFile(h, fileId)
                        Toast.makeText(this, "File deleted", Toast.LENGTH_SHORT).show()
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
