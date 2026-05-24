package chat.progressive.app.features.delta

import android.Manifest
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.media.MediaRecorder
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.NotificationChannelCompat
import androidx.core.app.NotificationCompat
import androidx.core.app.NotificationManagerCompat
import androidx.core.content.ContextCompat
import androidx.core.content.FileProvider
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import chat.progressive.app.delta.DeltaSessionHolder
import chat.progressive.app.protocol.IProtocolMessage
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.R
import im.vector.app.databinding.ActivityDeltaChatBinding
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.File
import java.io.FileOutputStream
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale
import javax.inject.Inject

@AndroidEntryPoint
class DeltaChatActivity : AppCompatActivity() {

    @Inject lateinit var deltaSessionHolder: DeltaSessionHolder

    private lateinit var binding: ActivityDeltaChatBinding
    private lateinit var adapter: MessageAdapter
    private var chatId: Long = 0
    private var lastDraft: String = ""
    private var isRecording = false
    private var mediaRecorder: MediaRecorder? = null
    private var voiceFile: File? = null
    private var cameraFile: File? = null

    companion object {
        private const val EXTRA_CHAT_ID = "chat_id"
        private const val CHANNEL_ID = "deltachat_messages"
        private const val NOTIFY_ID_BASE = 7000
        private val EMOJIS = arrayOf("👍", "❤️", "😂", "😮", "😢", "🙏", "👋", "🔥", "🎉", "✅")

        fun newIntent(context: Context, chatId: Long): Intent {
            return Intent(context, DeltaChatActivity::class.java).apply { putExtra(EXTRA_CHAT_ID, chatId) }
        }
    }

    private val pickMedia = registerForActivityResult(ActivityResultContracts.GetContent()) { uri: Uri? ->
        uri?.let { sendMedia(it) }
    }
    private val takePhoto = registerForActivityResult(ActivityResultContracts.TakePicture()) { success ->
        if (success) cameraFile?.let { sendMedia(Uri.fromFile(it)) }
    }
    private val requestPerms = registerForActivityResult(ActivityResultContracts.RequestMultiplePermissions()) { grants ->
        if (grants[Manifest.permission.RECORD_AUDIO] == true) startVoiceRecording()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityDeltaChatBinding.inflate(layoutInflater)
        setContentView(binding.root)
        createNotificationChannel()

        chatId = intent.getLongExtra(EXTRA_CHAT_ID, 0)
        binding.deltaBackButton.setOnClickListener { saveDraft(); finish() }
        binding.deltaMessageList.layoutManager = LinearLayoutManager(this).apply { stackFromEnd = true }
        adapter = MessageAdapter(
            onLongClick = { msg -> showMessageMenu(msg) },
            onReaction = { msg, emoji -> sendReaction(msg, emoji) }
        )
        binding.deltaMessageList.adapter = adapter

        val session = deltaSessionHolder.getSession()
        binding.deltaChatTitle.text = session?.getChat(chatId)?.title ?: "Chat"

        binding.deltaSendButton.setOnClickListener { sendMessage() }
        binding.deltaAttachButton.setOnClickListener { showAttachMenu() }
        binding.deltaChatMenu.setOnClickListener { showChatMenu() }
        binding.deltaProfileButton.setOnClickListener { startActivity(DeltaProfileActivity.newIntent(this)) }
        binding.deltaVoiceButton.setOnClickListener { toggleVoiceRecording() }

        setupReactionBar()
        restoreDraft()
        loadMessages()
        startNotificationLoop()
    }

    override fun onPause() { super.onPause(); saveDraft() }
    override fun onDestroy() { super.onDestroy(); stopVoiceRecording() }

    private fun setupReactionBar() {
        val bar = findViewById<LinearLayout>(R.id.deltaReactionBar)
        EMOJIS.forEach { emoji ->
            val tv = TextView(this).apply {
                text = emoji; textSize = 22f; setPadding(8, 4, 8, 4)
                setOnClickListener {
                    lifecycleScope.launch {
                        withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.sendReaction(chatId, 0, emoji) }
                        bar.visibility = View.GONE
                    }
                }
            }
            bar.addView(tv)
        }
    }

    private fun showAttachMenu() {
        AlertDialog.Builder(this)
            .setItems(arrayOf("📷 Camera", "🖼 Gallery", "📁 File", "🎤 Voice", "😀 Sticker")) { _, which ->
                when (which) {
                    0 -> openCamera()
                    1 -> pickMedia.launch("image/* video/*")
                    2 -> pickMedia.launch("*/*")
                    3 -> toggleVoiceRecording()
                    4 -> pickMedia.launch("image/*")
                }
            }.show()
    }

    private fun openCamera() {
        cameraFile = File(cacheDir, "dc_cam_${System.currentTimeMillis()}.jpg")
        takePhoto.launch(Uri.fromFile(cameraFile))
    }

    private fun toggleVoiceRecording() {
        if (isRecording) {
            stopVoiceRecording()
        } else {
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
                requestPerms.launch(arrayOf(Manifest.permission.RECORD_AUDIO))
            } else {
                startVoiceRecording()
            }
        }
    }

    private fun startVoiceRecording() {
        voiceFile = File(cacheDir, "dc_voice_${System.currentTimeMillis()}.ogg")
        mediaRecorder = MediaRecorder().apply {
            setAudioSource(MediaRecorder.AudioSource.MIC)
            setOutputFormat(MediaRecorder.OutputFormat.OGG)
            setAudioEncoder(MediaRecorder.AudioEncoder.OPUS)
            setOutputFile(voiceFile!!.absolutePath)
            prepare()
            start()
        }
        isRecording = true
        binding.deltaVoiceButton.text = "⏹"
        Toast.makeText(this, "Recording...", Toast.LENGTH_SHORT).show()
    }

    private fun stopVoiceRecording() {
        if (!isRecording) return
        try {
            mediaRecorder?.apply { stop(); release() }
            mediaRecorder = null
            isRecording = false
            binding.deltaVoiceButton.text = "🎤"
            voiceFile?.let { file ->
                lifecycleScope.launch {
                    withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.sendVoice(chatId, file.absolutePath) }
                    loadMessages()
                }
            }
        } catch (_: Exception) {}
    }

    private fun sendReaction(msg: IProtocolMessage, emoji: String) {
        lifecycleScope.launch {
            withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.sendReaction(chatId, msg.id, emoji) }
            loadMessages()
        }
    }

    private fun sendMedia(uri: Uri) {
        lifecycleScope.launch {
            val path = withContext(Dispatchers.IO) { copyUriToTemp(uri) }
            if (path != null) {
                withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.sendFile(chatId, path) }
                loadMessages()
                Toast.makeText(this@DeltaChatActivity, "Sent", Toast.LENGTH_SHORT).show()
            }
        }
    }

    private fun copyUriToTemp(uri: Uri): String? {
        return try {
            val input = contentResolver.openInputStream(uri) ?: return null
            val ext = contentResolver.getType(uri)?.let { mime ->
                when { mime.startsWith("image/") -> ".jpg"; mime.startsWith("video/") -> ".mp4"; mime.startsWith("audio/") -> ".ogg"; else -> ".dat" }
            } ?: ".dat"
            val file = File(cacheDir, "dc_${System.currentTimeMillis()}$ext")
            FileOutputStream(file).use { out -> input.copyTo(out) }; input.close()
            file.absolutePath
        } catch (_: Exception) { null }
    }

    private fun restoreDraft() {
        lastDraft = deltaSessionHolder.getSession()?.getConfig("draft_$chatId") ?: ""
        if (lastDraft.isNotEmpty()) binding.deltaMessageInput.setText(lastDraft)
    }

    private fun saveDraft() {
        val text = binding.deltaMessageInput.text?.toString() ?: ""
        if (text != lastDraft) { deltaSessionHolder.getSession()?.setConfig("draft_$chatId", text); lastDraft = text }
    }

    private fun loadMessages() {
        lifecycleScope.launch {
            val session = deltaSessionHolder.getSession() ?: return@launch
            val msgs = withContext(Dispatchers.IO) { session.getChat(chatId)?.getMessages(50, 0) ?: emptyList() }
            adapter.setMessages(msgs)
            if (adapter.itemCount > 0) binding.deltaMessageList.scrollToPosition(adapter.itemCount - 1)
        }
    }

    private fun sendMessage(quoteMsgId: Long = 0) {
        val text = binding.deltaMessageInput.text.toString().trim()
        if (text.isEmpty()) return
        binding.deltaMessageInput.text?.clear(); saveDraft()
        lifecycleScope.launch {
            withContext(Dispatchers.IO) {
                val final = if (quoteMsgId > 0) "> $text" else text
                deltaSessionHolder.getSession()?.sendMessage(chatId, final)
            }
            loadMessages()
        }
    }

    private fun showMessageMenu(msg: IProtocolMessage) {
        val items = mutableListOf("👍 React", "↩ Reply", "↗ Forward", "📋 Copy")
        if (msg.isOutgoing) items.addAll(listOf("✏ Edit", "🗑 Delete"))
        items.addAll(listOf("🔇 Mute", "📸 View Media"))

        Builder(this).setItems(items.toTypedArray()) { _, which ->
            when (items[which]) {
                "👍 React" -> showReactionPicker(msg)
                "↩ Reply" -> {
                    binding.deltaMessageInput.setText("> ${msg.text.take(50)}\n")
                    binding.deltaMessageInput.setSelection(binding.deltaMessageInput.text!!.length)
                }
                "↗ Forward" -> forwardMessage(msg)
                "📋 Copy" -> {
                    (getSystemService(Context.CLIPBOARD_SERVICE) as android.content.ClipboardManager)
                        .setPrimaryClip(android.content.ClipData.newPlainText("msg", msg.text))
                    Toast.makeText(this, "Copied", Toast.LENGTH_SHORT).show()
                }
                "✏ Edit" -> showEditDialog(msg)
                "🗑 Delete" -> {
                    lifecycleScope.launch { withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.deleteChat(chatId) }; loadMessages() }
                }
                "🔇 Mute" -> showMuteDialog()
                "📸 View Media" -> showMediaGallery()
            }
        }.show()
    }

    private fun showReactionPicker(msg: IProtocolMessage) {
        Builder(this).setTitle("React")
            .setItems(EMOJIS) { _, which ->
                sendReaction(msg, EMOJIS[which])
            }.show()
    }

    private fun showEditDialog(msg: IProtocolMessage) {
        val input = EditText(this).apply { setText(msg.text); setSelection(msg.text.length) }
        AlertDialog.Builder(this).setTitle("Edit Message").setView(input)
            .setPositiveButton("Save") { _, _ ->
                lifecycleScope.launch {
                    withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.editMessage(chatId, msg.id, input.text.toString()) }
                    loadMessages()
                }
            }.setNegativeButton("Cancel", null).show()
    }

    private fun showMuteDialog() {
        Builder(this).setTitle("Mute Chat")
            .setItems(arrayOf("1 hour", "8 hours", "1 day", "1 week", "Forever", "Unmute")) { _, which ->
                val ms = when (which) {
                    0 -> 3600000L; 1 -> 28800000L; 2 -> 86400000L; 3 -> 604800000L; 4 -> Long.MAX_VALUE; else -> 0L
                }
                deltaSessionHolder.getSession()?.muteChat(chatId, ms)
                Toast.makeText(this, if (ms == 0L) "Unmuted" else "Muted", Toast.LENGTH_SHORT).show()
            }.show()
    }

    private fun showMediaGallery() {
        lifecycleScope.launch {
            val media = withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.getChatMedia(chatId) ?: "[]" }
            Builder(this@DeltaChatActivity).setTitle("Media").setMessage("Media: $media").setPositiveButton("OK", null).show()
        }
    }

    private fun forwardMessage(msg: IProtocolMessage) {
        lifecycleScope.launch {
            val chats = withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.getChats() ?: emptyList() }
            Builder(this@DeltaChatActivity).setTitle("Forward to").setItems(chats.map { it.title }.toTypedArray()) { _, which ->
                lifecycleScope.launch {
                    withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.sendMessage(chats[which].id, msg.text) }
                    Toast.makeText(this@DeltaChatActivity, "Forwarded", Toast.LENGTH_SHORT).show()
                }
            }.show()
        }
    }

    // --- Notification loop (kept from previous) ---
    private fun createNotificationChannel() {
        NotificationManagerCompat.from(this).createNotificationChannel(
            NotificationChannelCompat.Builder(CHANNEL_ID, NotificationManagerCompat.IMPORTANCE_HIGH)
                .setName("DeltaChat Messages").setDescription("New messages").build())
    }
    private fun startNotificationLoop() {
        lifecycleScope.launch(Dispatchers.IO) {
            var lastMsgId = 0L
            while (isActive) {
                delay(3000)
                val s = deltaSessionHolder.getSession() ?: continue
                val msgs = s.getChat(chatId)?.getMessages(1, 0) ?: continue
                val latest = msgs.lastOrNull() ?: continue
                if (latest.id != lastMsgId && !latest.isOutgoing) {
                    lastMsgId = latest.id
                    withContext(Dispatchers.Main) {
                        NotificationManagerCompat.from(this@DeltaChatActivity).notify(NOTIFY_ID_BASE + chatId.toInt(),
                            NotificationCompat.Builder(this@DeltaChatActivity, CHANNEL_ID)
                                .setSmallIcon(R.drawable.ic_delta).setContentTitle(latest.senderName).setContentText(latest.text)
                                .setPriority(NotificationCompat.PRIORITY_HIGH).setAutoCancel(true)
                                .setContentIntent(android.app.PendingIntent.getActivity(this@DeltaChatActivity, 0, intent,
                                    android.app.PendingIntent.FLAG_UPDATE_CURRENT or android.app.PendingIntent.FLAG_IMMUTABLE)).build())
                        loadMessages()
                    }
                }
            }
        }
    }

    // --- Chat menu (9 actions, kept from previous) ---
    private fun showChatMenu() {
        Builder(this).setTitle(binding.deltaChatTitle.text).setItems(arrayOf(
            "Add Contact", "Create Group", "Verified Group (QR)", "Set Chat Name",
            "Backup (Export)", "Restore (Import)", "Key Transfer", "Connectivity Info", "Delete Chat"
        )) { _, which ->
            when (which) {
                0 -> showAddContact(); 1 -> showCreateGroup(); 2 -> showVerifiedGroup()
                3 -> showSetName(); 4 -> showBackup(); 5 -> showRestore()
                6 -> showKeyTransfer(); 7 -> showConnectivity(); 8 -> showDeleteChat()
            }
        }.show()
    }
    private fun showAddContact() {
        val l = LinearLayout(this).apply { orientation = LinearLayout.VERTICAL }
        val e = EditText(this).apply { hint = "Email" }; val n = EditText(this).apply { hint = "Name" }; l.addView(n); l.addView(e)
        Builder(this).setTitle("Add Contact").setView(l)
            .setPositiveButton("Add") { _, _ -> deltaSessionHolder.getSession()?.createContact(n.text.toString(), e.text.toString()); Toast.makeText(this, "Added", Toast.LENGTH_SHORT).show() }
            .setNegativeButton("Cancel", null).show()
    }
    private fun showCreateGroup() {
        val i = EditText(this).apply { hint = "Group name" }
        Builder(this).setTitle("Create Group").setView(i)
            .setPositiveButton("Create") { _, _ -> deltaSessionHolder.getSession()?.createGroupChat(false, i.text.toString()); Toast.makeText(this, "Created", Toast.LENGTH_SHORT).show() }
            .setNegativeButton("Cancel", null).show()
    }
    private fun showVerifiedGroup() {
        val qr = deltaSessionHolder.getSession()?.getSecureJoinQr(chatId) ?: ""
        Builder(this).setTitle("Verified Group").setMessage(if (qr.isNotEmpty()) "QR: $qr" else "No QR available")
            .setPositiveButton("Scan QR") { _, _ -> Toast.makeText(this, "QR scanner not integrated", Toast.LENGTH_SHORT).show() }
            .setNegativeButton("Close", null).show()
    }
    private fun showSetName() {
        val i = EditText(this).apply { setText(binding.deltaChatTitle.text) }
        Builder(this).setTitle("Set Chat Name").setView(i)
            .setPositiveButton("OK") { _, _ -> deltaSessionHolder.getSession()?.setChatName(chatId, i.text.toString()); binding.deltaChatTitle.text = i.text.toString() }
            .setNegativeButton("Cancel", null).show()
    }
    private fun showBackup() {
        lifecycleScope.launch {
            val r = withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.imex(1) }
            Builder(this@DeltaChatActivity).setTitle("Backup").setMessage(r ?: "Started").setPositiveButton("OK", null).show()
        }
    }
    private fun showRestore() {
        val i = EditText(this).apply { hint = "Backup file path" }
        Builder(this).setTitle("Restore").setView(i)
            .setPositiveButton("Restore") { _, _ -> lifecycleScope.launch { withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.imex(2, i.text.toString()) }; loadMessages() } }
            .setNegativeButton("Cancel", null).show()
    }
    private fun showKeyTransfer() {
        lifecycleScope.launch {
            val r = withContext(Dispatchers.IO) { deltaSessionHolder.getSession()?.initiateKeyTransfer() }
            Builder(this@DeltaChatActivity).setTitle("Key Transfer").setMessage(r ?: "Sent").setPositiveButton("OK", null).show()
        }
    }
    private fun showConnectivity() {
        Builder(this).setTitle("Connectivity").setMessage(deltaSessionHolder.getSession()?.getConnectivityHtml() ?: "Unknown").setPositiveButton("OK", null).show()
    }
    private fun showDeleteChat() {
        Builder(this).setTitle("Delete Chat").setMessage("Delete this chat and all messages?")
            .setPositiveButton("Delete") { _, _ -> deltaSessionHolder.getSession()?.deleteChat(chatId); finish() }
            .setNegativeButton("Cancel", null).show()
    }

    private class MessageAdapter(
        private val onLongClick: (IProtocolMessage) -> Unit,
        private val onReaction: (IProtocolMessage, String) -> Unit
    ) : RecyclerView.Adapter<MessageAdapter.VH>() {
        private var messages = listOf<IProtocolMessage>()
        fun setMessages(msgs: List<IProtocolMessage>) { messages = msgs; notifyDataSetChanged() }
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int) = VH(LayoutInflater.from(parent.context).inflate(R.layout.item_delta_message, parent, false))
        override fun onBindViewHolder(h: VH, pos: Int) {
            val m = messages[pos]
            h.sender.text = m.senderName.ifEmpty { m.senderId }; h.text.text = m.text
            h.time.text = SimpleDateFormat("HH:mm", Locale.getDefault()).format(Date(m.timestamp * 1000))
            h.itemView.setOnLongClickListener { onLongClick(m); true }
            h.react.setOnClickListener { onReaction(m, "👍") }
        }
        override fun getItemCount() = messages.size
        inner class VH(view: View) : RecyclerView.ViewHolder(view) {
            val sender: TextView = view.findViewById(R.id.deltaMessageSender)
            val text: TextView = view.findViewById(R.id.deltaMessageText)
            val time: TextView = view.findViewById(R.id.deltaMessageTime)
            val react: TextView = view.findViewById(R.id.deltaMessageReact)
        }
    }
}

private typealias Builder = AlertDialog.Builder
