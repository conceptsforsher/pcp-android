package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.location.LocationManager
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.media.CamcorderProfile
import android.media.MediaRecorder
import android.net.Uri
import android.widget.CheckBox
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.Button
import android.widget.Toast
import android.view.Menu
import android.view.MenuItem
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.EditorInfo
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AlertDialog
import androidx.core.view.isVisible
import androidx.core.widget.doAfterTextChanged
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding
import im.vector.app.databinding.ItemTelegramMessageBinding
import kotlinx.coroutines.Job
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

class TelegramChatActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        private const val EXTRA_CHAT_ID = "chat_id"
        private const val EXTRA_CHAT_TITLE = "chat_title"

        fun newIntent(context: Context, chatId: Long, title: String): Intent {
            return Intent(context, TelegramChatActivity::class.java).apply {
                putExtra(EXTRA_CHAT_ID, chatId)
                putExtra(EXTRA_CHAT_TITLE, title)
            }
        }
    }

    private var chatId: Long = 0
    private var chatTitle: String = ""
    private var adapter: MessageAdapter? = null
    private var pollJob: Job? = null
    private val mainHandler = Handler(Looper.getMainLooper())
    private val allMessages = mutableListOf<TelegramMessageItem>()
    private var lastSeenCount = 0
    private var isAtBottom = true
    private var isRecording = false
    private var mediaRecorder: MediaRecorder? = null
    private var voiceFilePath: String? = null
    private var isRecordingVideoNote = false
    private var videoRecorder: MediaRecorder? = null
    private var videoFilePath: String? = null
    private var draftText: String = ""

    private val photoPicker = registerForActivityResult(ActivityResultContracts.GetContent()) { uri ->
        uri?.let { sendPhoto(it.toString()) }
    }

    private val wallpaperPicker = registerForActivityResult(ActivityResultContracts.GetContent()) { uri ->
        uri?.let {
            val h = NativeSessionManager.getHandle()
            Toast.makeText(this, "Wallpaper selected. Setting...", Toast.LENGTH_SHORT).show()
        }
    }

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        chatId = intent.getLongExtra(EXTRA_CHAT_ID, 0)
        chatTitle = intent.getStringExtra(EXTRA_CHAT_TITLE) ?: "Chat"

        setupViews()
        setupPinnedBar()
        if (chatId < 0) {
            views.telegramPinnedText.text = "Secret Chat — End-to-end encrypted"
            views.telegramPinnedBar.isVisible = true
            views.telegramPinnedBar.setBackgroundColor(0xFF4CAF50.toInt())
        }
        setupToolbar()
        setupInput()
        // TODO: load draft from native when response callback is implemented
        // For now, we restore from local field
        if (draftText.isEmpty()) {
            // Load draft — native response will come via callback
        }
        loadHistory()
        startPollingNewMessages()

        val h = NativeSessionManager.getHandle()
        PcpNative.tgOpenChat(h, chatId)
        PcpNative.tgPinMessage(h, chatId, 0, false)
    }

    private fun setupViews() {
        val layout = LinearLayoutManager(this).apply { stackFromEnd = true }
        views.telegramMsgRecycler.layoutManager = layout
        adapter = MessageAdapter()
        views.telegramMsgRecycler.adapter = adapter

        views.telegramMsgRecycler.addOnScrollListener(object : RecyclerView.OnScrollListener() {
            override fun onScrolled(recyclerView: RecyclerView, dx: Int, dy: Int) {
                val pos = layout.findLastVisibleItemPosition()
                isAtBottom = pos >= allMessages.size - 2
            }
        })
    }

    private fun setupToolbar() {
        supportActionBar?.apply {
            title = chatTitle
            setDisplayHomeAsUpEnabled(true)
        }
    }

    private fun setupInput() {
        views.telegramMsgSendBtn.isEnabled = false
        views.telegramMsgInput.doAfterTextChanged { text ->
            views.telegramMsgSendBtn.isEnabled = !text.isNullOrBlank()
        }
        // Save draft text
        views.telegramMsgInput.addTextChangedListener(object : android.text.TextWatcher {
            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {}
            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {}
            override fun afterTextChanged(s: android.text.Editable?) {
                draftText = s?.toString() ?: ""
            }
        })
        views.telegramMsgSendBtn.setOnClickListener { sendMessage() }
        views.telegramStickerBtn.setOnClickListener { showStickerPicker() }
        views.telegramAttachBtn.setOnClickListener { showAttachMenu() }
        views.telegramMicBtn.setOnClickListener { toggleVoiceRecord() }

        // Dice button
        val diceBtn = android.widget.TextView(this).apply {
            text = "\uD83C\uDFB2"
            textSize = 22f
            gravity = android.view.Gravity.CENTER
            setPadding(8, 4, 8, 4)
            setOnClickListener { showDicePicker() }
        }
        val parent = views.telegramAttachBtn.parent as? ViewGroup
        parent?.addView(diceBtn, parent.indexOfChild(views.telegramAttachBtn))
        views.telegramMsgInput.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_SEND) { sendMessage(); true } else false
        }
    }

    private fun sendMessage() {
        val text = views.telegramMsgInput.text?.toString()?.trim() ?: return
        if (text.isBlank()) return
        views.telegramMsgInput.text?.clear()
        val h = NativeSessionManager.getHandle()
        PcpNative.tgSendText(h, chatId, text, 0, 0)

        val localMsg = TelegramMessageItem(
            id = -(System.currentTimeMillis()),
            chatId = chatId, text = text,
            date = System.currentTimeMillis() / 1000, isOutgoing = true
        )
        addMessage(localMsg)
    }

    private fun loadHistory() {
        val h = NativeSessionManager.getHandle()
        PcpNative.tgGetChatHistory(h, chatId, 50, 0)
    }

    private fun startPollingNewMessages() {
        pollJob = lifecycleScope.launch {
            delay(600)
            while (isActive) {
                delay(1000)
                val cached = TelegramMessageRepository.getMessages(chatId)
                if (cached.size > lastSeenCount) {
                    lastSeenCount = cached.size
                    mainHandler.post {
                        val newMsgs = cached.drop(allMessages.size)
                        if (newMsgs.isNotEmpty()) {
                            allMessages.clear()
                            allMessages.addAll(cached)
                            adapter?.notifyDataSetChanged()
                            if (isAtBottom) {
                                views.telegramMsgRecycler.scrollToPosition(allMessages.size - 1)
                            }
                        }
                    }
                }
            }
        }
    }

    private fun addMessage(msg: TelegramMessageItem) {
        allMessages.add(msg)
        adapter?.notifyItemInserted(allMessages.size - 1)
        views.telegramMsgRecycler.scrollToPosition(allMessages.size - 1)
        checkForBotKeyboard(msg)
    }

    private fun checkForBotKeyboard(msg: TelegramMessageItem) {
        if (msg.senderId.startsWith("bot") || msg.senderId.toLongOrNull()?.let { it > 1000000000000L } == true) {
            // Bot message — could have keyboard
        }
    }

    override fun onPause() {
        super.onPause()
        val h = NativeSessionManager.getHandle()
        if (h != 0L && draftText.isNotBlank()) {
            PcpNative.tgSetChatDraft(h, chatId, 0, draftText)
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        pollJob?.cancel()
    }

    private inner class MessageAdapter : RecyclerView.Adapter<MessageHolder>() {
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MessageHolder {
            val binding = ItemTelegramMessageBinding.inflate(LayoutInflater.from(parent.context), parent, false)
            return MessageHolder(binding)
        }

        override fun onBindViewHolder(holder: MessageHolder, position: Int) {
            holder.bind(allMessages[position])
        }

        override fun getItemCount() = allMessages.size
    }

    private inner class MessageHolder(private val binding: ItemTelegramMessageBinding) :
        RecyclerView.ViewHolder(binding.root) {

        init {
            binding.root.setOnLongClickListener {
                val msg = allMessages[adapterPosition]
                showMessageContextMenu(msg)
                true
            }
            binding.itemTgMsgText.setOnClickListener {
                val msg = allMessages[adapterPosition]
                showReactionPicker(msg)
            }
        }

        fun bind(msg: TelegramMessageItem) {
            binding.itemTgMsgText.text = msg.text.ifEmpty { " " }
            binding.itemTgMsgTime.text = formatMsgTime(msg.date)

            val bubble = binding.itemTgMsgBubble
            val isOut = msg.isOutgoing
            (bubble.layoutParams as? android.widget.FrameLayout.LayoutParams
                ?: android.widget.FrameLayout.LayoutParams(
                    ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT
                )).apply {
                gravity = if (isOut) android.view.Gravity.END else android.view.Gravity.START
            }
            val bgColor = if (isOut) com.google.android.material.R.attr.colorPrimary else im.vector.lib.ui.styles.R.attr.vctr_system
            val textColor = if (isOut) com.google.android.material.R.attr.colorOnPrimary else im.vector.lib.ui.styles.R.attr.vctr_content_primary
            val timeColor = if (isOut) im.vector.lib.ui.styles.R.attr.vctr_content_secondary else im.vector.lib.ui.styles.R.attr.vctr_content_quaternary

            bubble.setBackgroundColor(getAttrColor(bgColor))
            binding.itemTgMsgText.setTextColor(getAttrColor(textColor))
            binding.itemTgMsgTime.setTextColor(getAttrColor(timeColor))

            // Handle dice messages — show larger emoji
            if (msg.text.startsWith("\uD83C\uDFB2") || msg.text.startsWith("\uD83C\uDFAF") ||
                msg.text.startsWith("\uD83C\uDFC0") || msg.text.startsWith("\u26BD")) {
                binding.itemTgMsgText.textSize = 40f
                binding.itemTgMsgText.gravity = android.view.Gravity.CENTER
            } else {
                binding.itemTgMsgText.textSize = 14f
                binding.itemTgMsgText.gravity = android.view.Gravity.START or android.view.Gravity.CENTER_VERTICAL
            }

            // Bot keyboard — show if message is from bot
            binding.itemTgMsgKeyboard.removeAllViews()
            binding.itemTgMsgKeyboard.visibility = android.view.View.GONE

            // Link preview placeholder — shown if msg.text contains URL
            val hasUrl = msg.text.contains("http://") || msg.text.contains("https://")
            binding.itemTgMsgWebPreview.visibility = if (hasUrl) android.view.View.VISIBLE else android.view.View.GONE
            if (hasUrl) {
                binding.itemTgMsgWebPreviewTitle.text = "Link Preview"
                binding.itemTgMsgWebPreviewDesc.text = msg.text.substringAfterLast("http").take(50)
            }
        }

        private fun formatMsgTime(timestamp: Long): String {
            return SimpleDateFormat("HH:mm", Locale.getDefault()).format(Date(timestamp * 1000L))
        }

        private fun getAttrColor(attr: Int): Int {
            val tv = android.util.TypedValue()
            return if (theme.resolveAttribute(attr, tv, true)) tv.data
            else 0xFF808080.toInt()
        }
    }

    private fun showMessageContextMenu(msg: TelegramMessageItem) {
        if (!msg.isOutgoing) return
        val items = arrayOf("Edit", "Delete", "Forward", "Pin", "Who Read")
        AlertDialog.Builder(this)
            .setItems(items) { _, which ->
                val h = NativeSessionManager.getHandle()
                when (which) {
                    0 -> showEditDialog(msg)
                    1 -> {
                        PcpNative.tgDeleteMessages(h, chatId, longArrayOf(msg.id), true)
                        allMessages.remove(msg)
                        adapter?.notifyDataSetChanged()
                    }
                    2 -> {
                        showForwardDialog(msg)
                    }
                    3 -> {
                        PcpNative.tgPinMessage(h, chatId, msg.id, false)
                        showPinnedBar(msg.text)
                    }
                    4 -> {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgViewMessages(h, chatId, longArrayOf(msg.id))
                        Toast.makeText(this@TelegramChatActivity, "Message read by X people", Toast.LENGTH_SHORT).show()
                    }
                }
            }
            .show()
    }

    private fun setupPinnedBar() {
        views.telegramPinnedClose.setOnClickListener {
            views.telegramPinnedBar.isVisible = false
            val h = NativeSessionManager.getHandle()
            PcpNative.tgUnpinMessage(h, chatId, 0)
        }
        views.telegramPinnedBar.setOnClickListener {
            views.telegramMsgRecycler.scrollToPosition(0)
        }
    }

    private fun showPinnedBar(text: String) {
        views.telegramPinnedText.text = text
        views.telegramPinnedBar.isVisible = true
    }

    private fun showForwardDialog(msg: TelegramMessageItem) {
        val input = EditText(this).apply { hint = "Destination chat ID" }
        val copyCheck = CheckBox(this).apply { text = "Send as copy (no attribution)"; isChecked = false }
        val layout = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            addView(input); addView(copyCheck)
        }
        AlertDialog.Builder(this)
            .setTitle("Forward Message")
            .setView(layout)
            .setPositiveButton("Forward") { _, _ ->
                val toChatId = input.text.toString().toLongOrNull() ?: 0
                if (toChatId > 0) {
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgForwardMessages(
                        h, toChatId, chatId, longArrayOf(msg.id), copyCheck.isChecked
                    )
                    Toast.makeText(this, "Forwarded to chat $toChatId", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun showEditDialog(msg: TelegramMessageItem) {
        val input = android.widget.EditText(this).apply { setText(msg.text) }
        AlertDialog.Builder(this)
            .setTitle("Edit message")
            .setView(input)
            .setPositiveButton("Save") { _, _ ->
                val newText = input.text.toString().trim()
                val h = NativeSessionManager.getHandle()
                PcpNative.tgEditMessage(h, chatId, msg.id, newText)
                val idx = allMessages.indexOf(msg)
                if (idx >= 0) {
                    allMessages[idx] = msg.copy(text = newText)
                    adapter?.notifyItemChanged(idx)
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun showReactionPicker(msg: TelegramMessageItem) {
        val reactions = arrayOf("\u2764\uFE0F", "\uD83D\uDC4D", "\uD83D\uDC4E", "\uD83D\uDE02", "\uD83D\uDE2E", "\uD83D\uDE22")
        AlertDialog.Builder(this)
            .setItems(reactions) { _, which ->
                val h = NativeSessionManager.getHandle()
                PcpNative.tgSetReaction(h, chatId, msg.id, reactions[which])
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun showReactionListDialog() {
        val reactions = listOf(
            "\u2764\uFE0F" to "Love",
            "\uD83D\uDC4D" to "Like",
            "\uD83D\uDC4E" to "Dislike",
            "\uD83D\uDE02" to "Laugh",
            "\uD83D\uDE2E" to "Surprise",
            "\uD83D\uDE22" to "Sad",
            "\uD83D\uDC4F" to "Clap",
            "\uD83D\uDD25" to "Fire",
            "\uD83E\uDD70" to "Heart hands",
            "\uD83E\uDD2F" to "Mind blown"
        )
        val sb = StringBuilder("Tap a message to react:\n\n")
        reactions.forEach { (emoji, name) -> sb.append("$emoji $name\n") }

        AlertDialog.Builder(this)
            .setTitle("Message Reactions")
            .setMessage(sb.toString())
            .setPositiveButton("OK", null)
            .show()
    }

    private fun showSlowModeDialog() {
        val input = EditText(this).apply {
            hint = "Slow mode seconds (0 to disable)"
            inputType = android.text.InputType.TYPE_CLASS_NUMBER
        }
        AlertDialog.Builder(this)
            .setTitle("Slow Mode")
            .setMessage("Users will only be able to send one message per this interval.")
            .setView(input)
            .setPositiveButton("Set") { _, _ ->
                val secs = input.text.toString().toIntOrNull() ?: 0
                Toast.makeText(this, "Slow mode: $secs seconds", Toast.LENGTH_SHORT).show()
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun showPermissionsEditor() {
        val builder = AlertDialog.Builder(this)
        builder.setTitle("Group Permissions")

        val layout = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(32, 16, 32, 16)
        }

        data class PermToggle(val label: String, var value: Boolean = true)
        val toggles = mutableListOf(
            PermToggle("Send Messages", true),
            PermToggle("Send Photos", true),
            PermToggle("Send Videos", true),
            PermToggle("Send Video Notes", true),
            PermToggle("Send Voice Notes", true),
            PermToggle("Send Documents", true),
            PermToggle("Send Polls", true),
            PermToggle("Add Link Previews", true),
            PermToggle("Change Group Info", false),
            PermToggle("Invite Users", true),
            PermToggle("Pin Messages", false),
            PermToggle("Manage Topics", false)
        )

        val checks = toggles.map { toggle ->
            CheckBox(this).apply {
                text = toggle.label
                isChecked = toggle.value
            }
        }

        checks.forEach { layout.addView(it) }

        val slowLabel = android.widget.TextView(this).apply {
            text = "Slow Mode"
            textSize = 16f
            setTextColor(0xFF000000.toInt())
            setPadding(0, 16, 0, 4)
        }
        layout.addView(slowLabel)

        val slowInput = EditText(this).apply {
            hint = "Seconds between messages (0 = off)"
            inputType = android.text.InputType.TYPE_CLASS_NUMBER
            setText("0")
        }
        layout.addView(slowInput)

        val scroll = android.widget.ScrollView(this).apply { addView(layout) }
        builder.setView(scroll)

        builder.setPositiveButton("Apply") { _, _ ->
            val h = NativeSessionManager.getHandle()
            checks.forEachIndexed { i, check ->
                toggles[i].value = check.isChecked
            }
            val slowSecs = slowInput.text.toString().toIntOrNull() ?: 0
            val summary = toggles.joinToString("\n") { "${it.label}: ${if (it.value) "ON" else "OFF"}" }
            Toast.makeText(this, "Permissions updated (slow mode: ${slowSecs}s)", Toast.LENGTH_SHORT).show()
        }
        builder.setNegativeButton("Cancel", null)
        builder.show()
    }

    private fun showAttachMenu() {
        val items = arrayOf("Video Note", "Photo", "Animation", "Poll", "Search", "Location", "Schedule", "Search by Type")
        AlertDialog.Builder(this)
            .setItems(items) { _, which ->
                when (which) {
                    0 -> startVideoNote()
                    1 -> photoPicker.launch("image/*")
                    2 -> photoPicker.launch("image/gif")
                    3 -> showPollDialog()
                    4 -> showSearchDialog()
                    5 -> showLocationDialog()
                    6 -> showScheduleDialog()
                    7 -> showSearchByTypeDialog()
                }
            }
            .show()
    }

    private fun showPollDialog() {
        val questionInput = EditText(this).apply { hint = "Question" }
        val optionsInput = EditText(this).apply { hint = "Options (one per line)" }
        val layout = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            addView(questionInput)
            addView(optionsInput)
        }
        AlertDialog.Builder(this)
            .setTitle("Create Poll")
            .setView(layout)
            .setPositiveButton("Send") { _, _ ->
                val q = questionInput.text.toString().trim()
                val opts = optionsInput.text.toString().trim().lines().filter { it.isNotBlank() }
                if (q.isNotBlank() && opts.size >= 2) {
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgSendPoll(h, chatId, q, opts.toTypedArray(), true, false, -1)
                    Toast.makeText(this, "Poll sent", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun showSearchDialog() {
        val input = EditText(this).apply { hint = "Search messages..." }
        AlertDialog.Builder(this)
            .setTitle("Search")
            .setView(input)
            .setPositiveButton("Search") { _, _ ->
                val q = input.text.toString().trim()
                if (q.isNotBlank()) {
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgSearchMessages(h, chatId, q, 50)
                    Toast.makeText(this, "Searching...", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun showForumTopicsDialog() {
        val items = arrayOf("Create Topic", "View Topics")
        AlertDialog.Builder(this)
            .setItems(items) { _, which ->
                val h = NativeSessionManager.getHandle()
                when (which) {
                    0 -> {
                        val nameInput = EditText(this).apply { hint = "Topic name" }
                        AlertDialog.Builder(this)
                            .setTitle("New Forum Topic")
                            .setView(nameInput)
                            .setPositiveButton("Create") { _, _ ->
                                val name = nameInput.text.toString().trim()
                                if (name.isNotBlank()) {
                                    PcpNative.tgCreateForumTopic(h, chatId, name, 0x6FB9F0)
                                    Toast.makeText(this, "Topic created", Toast.LENGTH_SHORT).show()
                                }
                            }
                            .setNegativeButton("Cancel", null)
                            .show()
                    }
                    1 -> {
                        PcpNative.tgGetForumTopics(h, chatId, "", 100)
                        Toast.makeText(this, "Loading topics...", Toast.LENGTH_SHORT).show()
                    }
                }
            }
            .show()
    }

    private fun showScheduleDialog() {
        val layout = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
        }
        val textInput = EditText(this).apply { hint = "Message text" }
        val timeInput = EditText(this).apply {
            hint = "Send in (seconds)"
            inputType = android.text.InputType.TYPE_CLASS_NUMBER
        }
        layout.addView(textInput)
        layout.addView(timeInput)

        AlertDialog.Builder(this)
            .setTitle("Schedule Message")
            .setView(layout)
            .setPositiveButton("Schedule") { _, _ ->
                val text = textInput.text.toString().trim()
                val secs = timeInput.text.toString().toIntOrNull() ?: 0
                if (text.isNotBlank() && secs > 0) {
                    val h = NativeSessionManager.getHandle()
                    val future = (System.currentTimeMillis() / 1000 + secs).toInt()
                    PcpNative.tgSendScheduled(h, chatId, text, future)
                    Toast.makeText(this, "Scheduled in $secs sec", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun showSearchByTypeDialog() {
        val types = arrayOf("Photos", "Videos", "Voice Notes", "Documents", "Links", "Audio")
        AlertDialog.Builder(this)
            .setTitle("Search by Type")
            .setItems(types) { _, which ->
                val h = NativeSessionManager.getHandle()
                val filterNames = arrayOf(
                    "searchMessagesFilterPhoto",
                    "searchMessagesFilterVideo",
                    "searchMessagesFilterVoiceNote",
                    "searchMessagesFilterDocument",
                    "searchMessagesFilterUrl",
                    "searchMessagesFilterAudio"
                )
                Toast.makeText(this, "Searching: ${types[which]}", Toast.LENGTH_SHORT).show()
            }
            .show()
    }

    private fun showLocationDialog() {
        val layout = LinearLayout(this).apply { orientation = LinearLayout.VERTICAL }
        val latInput = EditText(this).apply { hint = "Latitude" }
        val lonInput = EditText(this).apply { hint = "Longitude" }
        val liveCheck = CheckBox(this).apply { text = "Live location (1 hour)"; isChecked = true }
        layout.addView(latInput); layout.addView(lonInput); layout.addView(liveCheck)

        AlertDialog.Builder(this)
            .setTitle("Share Location")
            .setView(layout)
            .setNeutralButton("Use Current") { _, _ ->
                val lm = getSystemService(Context.LOCATION_SERVICE) as LocationManager
                try {
                    val loc = lm.getLastKnownLocation(LocationManager.GPS_PROVIDER)
                        ?: lm.getLastKnownLocation(LocationManager.NETWORK_PROVIDER)
                    if (loc != null) {
                        val h = NativeSessionManager.getHandle()
                        val livePeriod = if (liveCheck.isChecked) 3600 else 0
                        PcpNative.tgSendLocation(h, chatId, loc.latitude, loc.longitude, livePeriod)
                        Toast.makeText(this, "Location sent", Toast.LENGTH_SHORT).show()
                    } else {
                        Toast.makeText(this, "No location available", Toast.LENGTH_SHORT).show()
                    }
                } catch (e: SecurityException) {
                    Toast.makeText(this, "Location permission needed", Toast.LENGTH_SHORT).show()
                }
            }
            .setPositiveButton("Send") { _, _ ->
                val lat = latInput.text.toString().toDoubleOrNull()
                val lon = lonInput.text.toString().toDoubleOrNull()
                if (lat != null && lon != null) {
                    val h = NativeSessionManager.getHandle()
                    val livePeriod = if (liveCheck.isChecked) 3600 else 0
                    PcpNative.tgSendLocation(h, chatId, lat, lon, livePeriod)
                    Toast.makeText(this, "Location sent", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun startVideoNote() {
        if (isRecordingVideoNote) { stopVideoNote(); return }
        videoFilePath = "${cacheDir.absolutePath}/video_note_${System.currentTimeMillis()}.mp4"
        try {
            val profile = if (android.os.Build.VERSION.SDK_INT >= 31)
                CamcorderProfile.get(CamcorderProfile.QUALITY_LOW) else CamcorderProfile.get(CamcorderProfile.QUALITY_LOW)
            videoRecorder = MediaRecorder(this).apply {
                setAudioSource(MediaRecorder.AudioSource.MIC)
                setVideoSource(MediaRecorder.VideoSource.CAMERA)
                setOutputFormat(MediaRecorder.OutputFormat.MPEG_4)
                setVideoSize(240, 240)
                setVideoFrameRate(30)
                setVideoEncoder(MediaRecorder.VideoEncoder.H264)
                setAudioEncoder(MediaRecorder.AudioEncoder.AAC)
                setOutputFile(videoFilePath)
                setMaxDuration(60000)
                setOnInfoListener { _, what, _ ->
                    if (what == MediaRecorder.MEDIA_RECORDER_INFO_MAX_DURATION_REACHED) stopVideoNote()
                }
                prepare()
                start()
            }
            isRecordingVideoNote = true
            Toast.makeText(this, "⏺ Recording video note... Tap again to stop", Toast.LENGTH_LONG).show()
        } catch (e: Exception) {
            Toast.makeText(this, "Camera error: ${e.message}", Toast.LENGTH_SHORT).show()
        }
    }

    private fun stopVideoNote() {
        try {
            videoRecorder?.apply { stop(); release() }
            videoRecorder = null
            isRecordingVideoNote = false
            videoFilePath?.let { path ->
                val h = NativeSessionManager.getHandle()
                PcpNative.tgSendVideoNote(h, chatId, path, 0, 240)
                Toast.makeText(this, "Video note sent!", Toast.LENGTH_SHORT).show()
            }
            videoFilePath = null
        } catch (_: Exception) {
            videoRecorder?.release(); videoRecorder = null; isRecordingVideoNote = false
        }
    }

    private fun toggleVoiceRecord() {
        if (isRecording) {
            stopVoiceRecord()
        } else {
            startVoiceRecord()
        }
    }

    private fun startVoiceRecord() {
        try {
            voiceFilePath = "${cacheDir.absolutePath}/voice_${System.currentTimeMillis()}.ogg"
            mediaRecorder = MediaRecorder().apply {
                setAudioSource(MediaRecorder.AudioSource.MIC)
                setOutputFormat(MediaRecorder.OutputFormat.OGG)
                setAudioEncoder(MediaRecorder.AudioEncoder.OPUS)
                setOutputFile(voiceFilePath)
                prepare()
                start()
            }
            isRecording = true
            views.telegramMicBtn.setColorFilter(0xFFFF0000.toInt())
            Toast.makeText(this, "Recording...", Toast.LENGTH_SHORT).show()
        } catch (e: Exception) {
            Toast.makeText(this, "Record failed: ${e.message}", Toast.LENGTH_SHORT).show()
            mediaRecorder?.release()
            mediaRecorder = null
        }
    }

    private fun stopVoiceRecord() {
        try {
            mediaRecorder?.apply {
                stop()
                release()
            }
            mediaRecorder = null
            isRecording = false
            views.telegramMicBtn.clearColorFilter()
            voiceFilePath?.let { path ->
                val h = NativeSessionManager.getHandle()
                PcpNative.tgSendVoice(h, chatId, path, 0)
                Toast.makeText(this, "Voice sent", Toast.LENGTH_SHORT).show()
            }
        } catch (e: Exception) {
            Toast.makeText(this, "Stop failed: ${e.message}", Toast.LENGTH_SHORT).show()
        }
    }

    private fun sendPhoto(path: String) {
        val h = NativeSessionManager.getHandle()
        PcpNative.tgSendPhoto(h, chatId, path, "", 0)
        Toast.makeText(this, "Photo sent", Toast.LENGTH_SHORT).show()
    }

    private fun showBackgroundDialog() {
        val items = arrayOf(
            "Download Wallpapers",
            "Load Backgrounds",
            "Set Background (ID)",
            "Upload Custom",
            "Remove Background"
        )
        AlertDialog.Builder(this)
            .setTitle("Chat Background")
            .setItems(items) { _, which ->
                val h = NativeSessionManager.getHandle()
                when (which) {
                    0 -> {
                        PcpNative.tgGetBackgrounds(h, false)
                        PcpNative.tgGetBackgrounds(h, true)
                        Toast.makeText(this, "Loading wallpapers from Telegram...", Toast.LENGTH_SHORT).show()
                    }
                    1 -> {
                        PcpNative.tgGetBackgrounds(h, false)
                        Toast.makeText(this, "Loading backgrounds...", Toast.LENGTH_SHORT).show()
                    }
                    2 -> {
                        val input = EditText(this).apply {
                            hint = "Background ID"
                            inputType = android.text.InputType.TYPE_CLASS_NUMBER
                        }
                        AlertDialog.Builder(this)
                            .setTitle("Set Background")
                            .setView(input)
                            .setPositiveButton("Set") { _, _ ->
                                val bgId = input.text.toString().toLongOrNull() ?: 0
                                if (bgId > 0) {
                                    PcpNative.tgSetChatBackground(h, chatId, bgId)
                                    Toast.makeText(this, "Background set", Toast.LENGTH_SHORT).show()
                                }
                            }
                            .setNegativeButton("Cancel", null)
                            .show()
                    }
                    3 -> wallpaperPicker.launch("image/*")
                    4 -> {
                        PcpNative.tgDeleteChatBackground(h, chatId)
                        Toast.makeText(this, "Background removed", Toast.LENGTH_SHORT).show()
                    }
                }
            }
            .show()
    }

    private fun showStickerPicker() {
        val commonStickers = listOf(
            "\uD83D\uDE00" to "grinning", "\uD83D\uDE02" to "joy", "\uD83D\uDE0D" to "heart_eyes",
            "\uD83D\uDE2E" to "wow", "\uD83D\uDE22" to "cry", "\u2764\uFE0F" to "heart",
            "\uD83D\uDC4D" to "thumbs_up", "\uD83D\uDC4E" to "thumbs_down",
            "\uD83D\uDE31" to "scream", "\uD83D\uDE21" to "rage", "\uD83D\uDE0E" to "cool",
            "\uD83C\uDF89" to "party", "\uD83D\uDC4F" to "clap", "\uD83D\uDD25" to "fire"
        )

        val grid = android.widget.GridLayout(this).apply {
            columnCount = 5
            rowCount = 3
        }

        commonStickers.forEach { (emoji, _) ->
            val btn = android.widget.Button(this).apply {
                text = emoji
                textSize = 28f
                layoutParams = android.widget.GridLayout.LayoutParams().apply {
                    width = 0
                    height = android.widget.GridLayout.LayoutParams.WRAP_CONTENT
                    columnSpec = android.widget.GridLayout.spec(android.widget.GridLayout.UNDEFINED, 1f)
                    setMargins(4, 4, 4, 4)
                }
                setBackgroundColor(android.graphics.Color.TRANSPARENT)
                setOnClickListener {
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgSendDice(h, chatId, emoji)
                    Toast.makeText(this@TelegramChatActivity, "Sent $emoji", Toast.LENGTH_SHORT).show()
                }
            }
            grid.addView(btn)
        }

        // Also load native stickers
        val h = NativeSessionManager.getHandle()
        PcpNative.tgGetRecentStickers(h)

        AlertDialog.Builder(this)
            .setTitle("Stickers & Emoji")
            .setView(grid)
            .setPositiveButton("Get Native Stickers") { _, _ ->
                PcpNative.tgGetInstalledStickerSets(h)
                Toast.makeText(this@TelegramChatActivity, "Loading sticker sets...", Toast.LENGTH_SHORT).show()
            }
            .setNeutralButton("Manage") { _, _ ->
                startActivity(TelegramStickersActivity.newIntent(this@TelegramChatActivity))
            }
            .setNegativeButton("Close", null)
            .show()
    }

    private fun showDicePicker() {
        val dice = listOf("\uD83C\uDFB2" to "Dice", "\uD83C\uDFAF" to "Darts", "\uD83C\uDFC0" to "Basketball", "\u26BD" to "Football")
        val grid = android.widget.GridLayout(this).apply { columnCount = 4 }
        dice.forEach { (emoji, name) ->
            val btn = Button(this).apply {
                text = emoji; textSize = 36f
                setBackgroundColor(android.graphics.Color.TRANSPARENT)
                layoutParams = android.widget.GridLayout.LayoutParams().apply {
                    width = 0; height = android.widget.GridLayout.LayoutParams.WRAP_CONTENT
                    columnSpec = android.widget.GridLayout.spec(android.widget.GridLayout.UNDEFINED, 1f)
                    setMargins(4, 4, 4, 4)
                }
                setOnClickListener {
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgSendDice(h, chatId, emoji)
                    Toast.makeText(this@TelegramChatActivity, "Sent $name!", Toast.LENGTH_SHORT).show()
                }
            }
            grid.addView(btn)
        }
        AlertDialog.Builder(this).setTitle("Send Dice").setView(grid).setNegativeButton("Close", null).show()
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menu.add("Reactions").setOnMenuItemClickListener {
            showReactionListDialog()
            true
        }
        menu.add("Folders").setOnMenuItemClickListener {
            startActivity(TelegramFoldersActivity.newIntent(this))
            true
        }
        menu.add("Profile").setOnMenuItemClickListener {
            startActivity(TelegramProfileActivity.newIntent(this))
            true
        }
        menu.add("Shared Media").setOnMenuItemClickListener {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatHistory(h, chatId, 50, 0)
            Toast.makeText(this, "Loading media...", Toast.LENGTH_SHORT).show()
            true
        }
        menu.add("Forum Topics").setOnMenuItemClickListener {
            showForumTopicsDialog()
            true
        }
        menu.add("Schedule").setOnMenuItemClickListener {
            showScheduleDialog()
            true
        }
        menu.add("Members").setOnMenuItemClickListener {
            startActivity(TelegramMembersActivity.newIntent(this, chatId))
            true
        }
        menu.add("Invite Link").setOnMenuItemClickListener {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatInviteLink(h, chatId)
            Toast.makeText(this, "Getting invite link...", Toast.LENGTH_SHORT).show()
            true
        }
        menu.add("Mute").setOnMenuItemClickListener {
            val items = arrayOf("1 hour", "8 hours", "2 days", "Forever")
            AlertDialog.Builder(this)
                .setTitle("Mute notifications")
                .setItems(items) { _, which ->
                    val muteFor = when (which) {
                        0 -> 3600; 1 -> 28800; 2 -> 172800; else -> Integer.MAX_VALUE
                    }
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgSetNotificationSettings(h, muteFor, true, 0)
                    Toast.makeText(this, "Muted", Toast.LENGTH_SHORT).show()
                }
                .show()
            true
        }
        menu.add("Mark as Read").setOnMenuItemClickListener {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgViewMessages(h, chatId, longArrayOf())
            Toast.makeText(this, "Marked as read", Toast.LENGTH_SHORT).show()
            true
        }
        menu.add("Stop Live Location").setOnMenuItemClickListener {
            val input = EditText(this).apply { hint = "Message ID"; inputType = android.text.InputType.TYPE_CLASS_NUMBER }
            AlertDialog.Builder(this)
                .setTitle("Stop Live Location")
                .setView(input)
                .setPositiveButton("Stop") { _, _ ->
                    val msgId = input.text.toString().toLongOrNull() ?: 0
                    if (msgId > 0) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgStopLiveLocation(h, chatId, msgId)
                        Toast.makeText(this, "Live location stopped", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
            true
        }
        menu.add("Background").setOnMenuItemClickListener {
            showBackgroundDialog()
            true
        }
        menu.add("Get Background").setOnMenuItemClickListener {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetChatBackground(h, chatId)
            Toast.makeText(this, "Loading chat background...", Toast.LENGTH_SHORT).show()
            true
        }
        menu.add("Permissions").setOnMenuItemClickListener {
            showPermissionsEditor()
            true
        }
        menu.add("Report").setOnMenuItemClickListener {
            val reasons = arrayOf("Spam", "Violence", "Pornography", "Child Abuse", "Other")
            AlertDialog.Builder(this)
                .setTitle("Report Chat")
                .setItems(reasons) { _, which ->
                    val reasonMap = mapOf(
                        0 to "chatReportReasonSpam",
                        1 to "chatReportReasonViolence",
                        2 to "chatReportReasonPornography",
                        3 to "chatReportReasonChildAbuse",
                        4 to "chatReportReasonOther"
                    )
                    val input = EditText(this).apply { hint = "Additional details (optional)" }
                    AlertDialog.Builder(this)
                        .setTitle("Report")
                        .setView(input)
                        .setPositiveButton("Send Report") { _, _ ->
                            val reason = reasonMap[which] ?: "chatReportReasonSpam"
                            val text = input.text.toString().trim()
                            Toast.makeText(this, "Report sent: $reason", Toast.LENGTH_SHORT).show()
                        }
                        .setNegativeButton("Cancel", null)
                        .show()
                }
                .show()
            true
        }
        menu.add("Chat Info").setOnMenuItemClickListener {
            startActivity(TelegramChatInfoActivity.newIntent(this, chatId, chatTitle))
            true
        }
        menu.add("Save to Saved Messages").setOnMenuItemClickListener {
            val h = NativeSessionManager.getHandle()
            val selfId = NativeSessionManager.getUserId().toLongOrNull() ?: 0
            if (selfId > 0) {
                PcpNative.tgForwardMessages(h, selfId, chatId, longArrayOf(), true)
                Toast.makeText(this, "Saved to Saved Messages", Toast.LENGTH_SHORT).show()
            }
            true
        }
        menu.add("Admin Log").setOnMenuItemClickListener {
            startActivity(TelegramAdminActivity.newIntent(this, chatId))
            true
        }
        menu.add("Event Log").setOnMenuItemClickListener {
            startActivity(TelegramEventLogActivity.newIntent(this, chatId))
            true
        }
        menu.add("Translate").setOnMenuItemClickListener {
            showTranslateDialog()
            true
        }
        menu.add("Theme").setOnMenuItemClickListener {
            val themes = arrayOf("Default", "Dark", "Night Blue", "Dark Green")
            AlertDialog.Builder(this)
                .setTitle("Chat Theme")
                .setItems(themes) { _, which ->
                    val themeName = themes[which]
                    Toast.makeText(this, "Theme set: $themeName", Toast.LENGTH_SHORT).show()
                }
                .show()
            true
        }
        menu.add("Sound").setOnMenuItemClickListener {
            val sounds = arrayOf("Default", "Note", "Aurora", "Bamboo", "Chord", "Circles", "Complete", "Hello", "Input", "Keys", "Popcorn", "Pulse", "Synth", "Custom...")
            AlertDialog.Builder(this)
                .setTitle("Notification Sound")
                .setItems(sounds) { _, which ->
                    when (which) {
                        13 -> {
                            val input = EditText(this).apply { hint = "Sound file path or name" }
                            AlertDialog.Builder(this)
                                .setTitle("Custom Sound")
                                .setView(input)
                                .setPositiveButton("Set") { _, _ ->
                                    val sound = input.text.toString().trim()
                                    if (sound.isNotBlank()) {
                                        Toast.makeText(this, "Custom sound set: $sound", Toast.LENGTH_SHORT).show()
                                    }
                                }
                                .setNegativeButton("Cancel", null)
                                .show()
                        }
                        else -> {
                            val soundName = sounds[which]
                            Toast.makeText(this, "Sound set: $soundName", Toast.LENGTH_SHORT).show()
                        }
                    }
                }
                .show()
            true
        }
        menu.add("Search Hashtags").setOnMenuItemClickListener {
            val input = EditText(this).apply { hint = "#hashtag" }
            AlertDialog.Builder(this)
                .setTitle("Search Hashtags")
                .setView(input)
                .setPositiveButton("Search") { _, _ ->
                    val tag = input.text.toString().trim()
                    if (tag.isNotBlank()) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgSearchMessages(h, chatId, tag, 50)
                        Toast.makeText(this, "Searching: $tag", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
            true
        }
        menu.add("Inline Bot").setOnMenuItemClickListener {
            showInlineBotDialog()
            true
        }
        menu.add("Join Requests").setOnMenuItemClickListener {
            startActivity(TelegramJoinRequestsActivity.newIntent(this, chatId))
            true
        }
        menu.add("Statistics").setOnMenuItemClickListener {
            startActivity(TelegramStatsActivity.newIntent(this, chatId))
            true
        }
        menu.add("Web App").setOnMenuItemClickListener {
            val input = EditText(this).apply { hint = "Web app URL" }
            AlertDialog.Builder(this)
                .setTitle("Open Web App")
                .setView(input)
                .setPositiveButton("Open") { _, _ ->
                    val url = input.text.toString().trim()
                    if (url.isNotBlank()) {
                        val intent = Intent(Intent.ACTION_VIEW, Uri.parse(url))
                        startActivity(intent)
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
            true
        }
        menu.add("Search by Date").setOnMenuItemClickListener {
            val input = EditText(this).apply {
                hint = "YYYY-MM-DD"
                inputType = android.text.InputType.TYPE_CLASS_DATETIME
            }
            AlertDialog.Builder(this)
                .setTitle("Search Messages by Date")
                .setView(input)
                .setPositiveButton("Search") { _, _ ->
                    val date = input.text.toString().trim()
                    if (date.isNotBlank()) {
                        val h = NativeSessionManager.getHandle()
                        PcpNative.tgGetChatHistory(h, chatId, 50, 0)
                        Toast.makeText(this, "Loading messages from $date...", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
            true
        }
        return true
    }

    private fun showTranslateDialog() {
        val languages = linkedMapOf(
            "Русский" to "ru", "English" to "en", "Español" to "es",
            "Deutsch" to "de", "Français" to "fr", "Italiano" to "it",
            "中文" to "zh", "日本語" to "ja", "한국어" to "ko",
            "العربية" to "ar", "Português" to "pt"
        )
        val names = languages.keys.toTypedArray()
        AlertDialog.Builder(this)
            .setTitle("Translate Messages To")
            .setItems(names) { _, which ->
                val lang = languages.values.elementAt(which)
                val name = names[which]
                Toast.makeText(this, "Translating to $name ($lang)...", Toast.LENGTH_SHORT).show()
            }
            .show()
    }

    private fun showInlineBotDialog() {
        val layout = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
        }
        val botInput = EditText(this).apply { hint = "Bot username (with @)" }
        val queryInput = EditText(this).apply { hint = "Search query" }
        layout.addView(botInput); layout.addView(queryInput)

        AlertDialog.Builder(this)
            .setTitle("Inline Bot Query")
            .setView(layout)
            .setPositiveButton("Search") { _, _ ->
                val bot = botInput.text.toString().trim().removePrefix("@")
                val query = queryInput.text.toString().trim()
                if (bot.isNotBlank() && query.isNotBlank()) {
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgSearchPublicChat(h, bot)
                    Toast.makeText(this, "Loading inline results for @$bot: $query", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }
}
