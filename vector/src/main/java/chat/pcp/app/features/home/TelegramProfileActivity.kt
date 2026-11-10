package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.view.View
import android.os.Looper
import android.view.ViewGroup
import android.widget.Button
import android.widget.EditText
import android.widget.GridLayout
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.core.view.isVisible
import androidx.lifecycle.lifecycleScope
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramProfileBinding
import im.vector.app.features.home.HomeActivity
import kotlinx.coroutines.Job
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import org.json.JSONObject
import timber.log.Timber

class TelegramProfileActivity : VectorBaseActivity<ActivityTelegramProfileBinding>() {

    companion object {
        fun newIntent(context: Context, userId: Long = 0): Intent {
            return Intent(context, TelegramProfileActivity::class.java).apply {
                putExtra("user_id", userId)
            }
        }
    }

    private var userId: Long = 0
    private var pollJob: Job? = null
    private val mainHandler = Handler(Looper.getMainLooper())
    private var profileJson: String? = null

    override fun getBinding() = ActivityTelegramProfileBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        userId = intent.getLongExtra("user_id", 0)
        supportActionBar?.apply {
            title = "Profile"
            setDisplayHomeAsUpEnabled(true)
        }

        setupButtons()
        setupButtonsVisibility()
        if (userId == 0L) {
            val accountBtn = android.widget.Button(this).apply {
                text = "Account Settings"
                setTextColor(0xFF000000.toInt())
                setOnClickListener { startActivity(TelegramAccountActivity.newIntent(this@TelegramProfileActivity)) }
            }
            val securityBtn = android.widget.Button(this).apply {
                text = "Security & 2FA"
                setTextColor(0xFF4CAF50.toInt())
                setOnClickListener { startActivity(TelegramSecurityActivity.newIntent(this@TelegramProfileActivity)) }
            }
            val container = views.root as? android.view.ViewGroup
            container?.addView(accountBtn)
            container?.addView(securityBtn)
        }
        loadProfile()
    }

    private fun setupButtons() {
        views.tgProfileEditBtn.setOnClickListener { showEditDialog() }
        views.tgProfileSharedBtn.setOnClickListener {
            showEmojiStatusDialog()
        }
        (views.tgProfileSharedBtn.getChildAt(0) as? android.widget.TextView)?.text = "Set Emoji Status"
        views.tgProfileGroupsBtn.setOnClickListener { loadGroupsInCommon() }
        views.tgProfileSessionsBtn.setOnClickListener { loadSessions() }
        views.tgProfileFoldersBtn.setOnClickListener {
            startActivity(TelegramFoldersActivity.newIntent(this))
        }
        val storyPrivacyBtn = Button(this).apply {
            layoutParams = ViewGroup.MarginLayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
            ).apply { topMargin = (8 * resources.displayMetrics.density).toInt() }
            text = "Story Privacy"
            setOnClickListener {
                val h = NativeSessionManager.getHandle()
                PcpNative.tgGetStoryPrivacy(h)
                Toast.makeText(this@TelegramProfileActivity, "Loading story privacy settings...", Toast.LENGTH_SHORT).show()
            }
        }
        (views.tgProfileFoldersBtn.parent as ViewGroup).addView(storyPrivacyBtn, (views.tgProfileFoldersBtn.parent as ViewGroup).indexOfChild(views.tgProfileFoldersBtn) + 1)
        views.tgProfileSecretChatBtn.setOnClickListener {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgCreateSecretChat(h, userId)
            Toast.makeText(this, "Secret chat request sent", Toast.LENGTH_SHORT).show()
        }
        views.tgProfileBlockBtn.setOnClickListener {
            AlertDialog.Builder(this)
                .setTitle("Block User?")
                .setMessage("You will no longer receive messages from this user.")
                .setPositiveButton("Block") { _, _ ->
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgBlockUser(h, userId)
                    Toast.makeText(this, "User blocked", Toast.LENGTH_SHORT).show()
                    finish()
                }
                .setNegativeButton("Cancel", null)
                .show()
        }
        views.tgProfileProxyBtn.setOnClickListener {
            startActivity(TelegramProxyActivity.newIntent(this))
        }
        views.tgProfileLogoutBtn.setOnClickListener { logout() }
    }

    private fun loadProfile() {
        val h = NativeSessionManager.getHandle()
        if (userId > 0) {
            PcpNative.tgGetUser(h, userId)
            PcpNative.tgGetUserProfilePhotos(h, userId, 0, 100)
        } else {
            PcpNative.tgGetMe(h)
        }
        val h2 = NativeSessionManager.getHandle()
        PcpNative.tgGetNotificationSettings(h2)
    }

    private fun setupButtonsVisibility() {
        views.tgProfileEditBtn.isVisible = userId == 0L
        views.tgProfileFoldersBtn.isVisible = userId == 0L
        views.tgProfileProxyBtn.isVisible = userId == 0L
        views.tgProfileLogoutBtn.isVisible = userId == 0L
        views.tgProfileSessionsBtn.isVisible = userId == 0L
        views.tgProfileSecretChatBtn.isVisible = userId > 0L
        views.tgProfileBlockBtn.isVisible = userId > 0L

        if (userId == 0L) {
            android.widget.LinearLayout(this).apply {
                orientation = android.widget.LinearLayout.VERTICAL
                setPadding(0, 16, 0, 0)
            }.let { buttonsContainer ->
                val privacyBtn = android.widget.Button(this).apply {
                    text = "Privacy & Security"
                    setTextColor(0xFF000000.toInt())
                    setOnClickListener { startActivity(TelegramPrivacyActivity.newIntent(this@TelegramProfileActivity)) }
                }
                buttonsContainer.addView(privacyBtn)
                val downloadsBtn = android.widget.Button(this).apply {
                    text = "Downloads"
                    setTextColor(0xFF000000.toInt())
                    setOnClickListener { startActivity(TelegramDownloadsActivity.newIntent(this@TelegramProfileActivity)) }
                }
                buttonsContainer.addView(downloadsBtn)
                val stickersBtn = android.widget.Button(this).apply {
                    text = "Stickers & Emoji"
                    setTextColor(0xFF000000.toInt())
                    setOnClickListener { startActivity(TelegramStickersActivity.newIntent(this@TelegramProfileActivity)) }
                }
                buttonsContainer.addView(stickersBtn)
                (views.root as? android.view.ViewGroup)?.addView(buttonsContainer)
            }

            val closeFriendsBtn = android.widget.Button(this).apply {
                text = "Close Friends"
                setTextColor(0xFF000000.toInt())
                setOnClickListener {
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgGetCloseFriends(h)
                    val input = android.widget.EditText(this@TelegramProfileActivity).apply { hint = "User IDs (comma-separated)" }
                    androidx.appcompat.app.AlertDialog.Builder(this@TelegramProfileActivity)
                        .setTitle("Set Close Friends")
                        .setView(input)
                        .setPositiveButton("Set") { _, _ ->
                            val ids = input.text.toString().split(",").mapNotNull { it.trim().toLongOrNull() }.toLongArray()
                            if (ids.isNotEmpty()) {
                                PcpNative.tgSetCloseFriends(h, ids)
                                Toast.makeText(this@TelegramProfileActivity, "Close friends updated", Toast.LENGTH_SHORT).show()
                            }
                        }
                        .setNegativeButton("Cancel", null)
                        .show()
                }
            }
            (views.root as? android.view.ViewGroup)?.addView(closeFriendsBtn)

            val qrBtn = android.widget.Button(this).apply {
                text = "Web Login QR"
                setTextColor(0xFF000000.toInt())
                setOnClickListener {
                    Toast.makeText(this@TelegramProfileActivity, "Web login QR would be displayed here.\nScan with Telegram mobile to log in on web.", Toast.LENGTH_LONG).show()
                }
            }
            (views.root as? android.view.ViewGroup)?.addView(qrBtn)

            val premiumBtn = android.widget.Button(this).apply {
                text = "\u2B50 Premium"
                setTextColor(0xFFFFA000.toInt())
                setOnClickListener {
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgGetPremiumState(h)
                    AlertDialog.Builder(this@TelegramProfileActivity)
                        .setTitle("Premium Features")
                        .setMessage("Telegram Premium unlocks:\n\n" +
                            "\u2022 4 GB file uploads\n" +
                            "\u2022 Faster downloads\n" +
                            "\u2022 Voice-to-text conversion\n" +
                            "\u2022 No ads\n" +
                            "\u2022 Advanced chat management\n" +
                            "\u2022 Animated emoji status\n" +
                            "\u2022 Premium stickers\n" +
                            "\u2022 Profile badge")
                        .setPositiveButton("OK", null)
                        .setNeutralButton("Check Status") { _, _ ->
                            Toast.makeText(this@TelegramProfileActivity, "Loading premium status...", Toast.LENGTH_SHORT).show()
                        }
                        .show()
                }
            }
            (views.root as? android.view.ViewGroup)?.addView(premiumBtn)
        }
    }

    private fun showEditDialog() {
        val items = arrayOf("Edit Name", "Edit Bio", "Edit Username")
        AlertDialog.Builder(this)
            .setItems(items) { _, which -> showFieldEdit(which) }
            .show()
    }

    private fun showFieldEdit(field: Int) {
        val input = EditText(this)
        val h = NativeSessionManager.getHandle()
        AlertDialog.Builder(this)
            .setTitle(when (field) { 0 -> "Name (First Last)"; 1 -> "Bio"; else -> "Username" })
            .setView(input)
            .setPositiveButton("Save") { _, _ ->
                val text = input.text.toString().trim()
                when (field) {
                    0 -> {
                        val parts = text.split(" ", limit = 2)
                        PcpNative.tgSetName(h, parts.getOrElse(0) { "" }, parts.getOrElse(1) { "" })
                    }
                    1 -> PcpNative.tgSetBio(h, text)
                    2 -> PcpNative.tgSetUsername(h, text)
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun showEmojiStatusDialog() {
        val emojis = listOf(
            "\uD83D\uDE00" to "grinning",
            "\u2764\uFE0F" to "heart",
            "\uD83D\uDC4D" to "thumbs_up",
            "\uD83D\uDD25" to "fire",
            "\uD83C\uDF89" to "party",
            "\u2B50" to "star",
            "\uD83D\uDC4C" to "ok",
            "\uD83D\uDE34" to "sleeping"
        )

        val grid = GridLayout(this).apply {
            columnCount = 4
            rowCount = 3
        }

        emojis.forEach { (emoji, _) ->
            val btn = Button(this).apply {
                text = emoji
                textSize = 32f
                layoutParams = GridLayout.LayoutParams().apply {
                    width = 0
                    height = GridLayout.LayoutParams.WRAP_CONTENT
                    columnSpec = GridLayout.spec(GridLayout.UNDEFINED, 1f)
                    setMargins(4, 4, 4, 4)
                }
                setBackgroundColor(android.graphics.Color.TRANSPARENT)
                setOnClickListener {
                    val h = NativeSessionManager.getHandle()
                    val emojiId = emoji.codePointAt(0).toLong()
                    PcpNative.tgSetEmojiStatus(h, emojiId, 0)
                    Toast.makeText(this@TelegramProfileActivity, "Emoji status set: $emoji", Toast.LENGTH_SHORT).show()
                }
            }
            grid.addView(btn)
        }

        AlertDialog.Builder(this)
            .setTitle("Set Emoji Status")
            .setView(grid)
            .setNeutralButton("Clear Status") { _, _ ->
                val h = NativeSessionManager.getHandle()
                PcpNative.tgClearEmojiStatus(h)
                Toast.makeText(this, "Emoji status cleared", Toast.LENGTH_SHORT).show()
            }
            .setNegativeButton("Close", null)
            .show()
    }

    private fun loadSharedMedia() {
        val h = NativeSessionManager.getHandle()
        val uid = userId.takeIf { it > 0 } ?: 0
        PcpNative.tgGetChatHistory(h, uid, 50, 0)
        android.widget.Toast.makeText(this, "Loading shared media...", android.widget.Toast.LENGTH_SHORT).show()
    }

    private fun loadGroupsInCommon() {
        val h = NativeSessionManager.getHandle()
        val uid = userId.takeIf { it > 0 } ?: 0
        if (uid > 0) {
            PcpNative.tgGetGroupsInCommon(h, uid, 0)
            android.widget.Toast.makeText(this, "Loading groups...", android.widget.Toast.LENGTH_SHORT).show()
        }
    }

    private fun loadSessions() {
        val h = NativeSessionManager.getHandle()
        PcpNative.tgGetActiveSessions(h)
        startPollingSessions()
    }

    private fun startPollingSessions() {
        pollJob?.cancel()
        pollJob = lifecycleScope.launch {
            delay(1500)
            AlertDialog.Builder(this@TelegramProfileActivity)
                .setTitle("Active Sessions")
                .setMessage("Session data received from native. Check logs.")
                .setPositiveButton("Terminate All Others") { _, _ ->
                    PcpNative.tgTerminateAllOtherSessions(
                        NativeSessionManager.getHandle()
                    )
                }
                .setNegativeButton("Close", null)
                .show()
        }
    }

    private fun logout() {
        AlertDialog.Builder(this)
            .setTitle("Log Out from Telegram?")
            .setMessage("You will need to re-enter your phone number to log back in.")
            .setPositiveButton("Log Out") { _, _ ->
                val h = NativeSessionManager.getHandle()
                PcpNative.tgLogout(h)
                NativeSessionManager.clear()
                finishAffinity()
                startActivity(HomeActivity.newIntent(this, firstStartMainActivity = true))
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    override fun onDestroy() {
        super.onDestroy()
        pollJob?.cancel()
    }
}
