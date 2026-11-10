package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.View
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.core.view.isVisible
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramChatBinding

class TelegramStoryViewerActivity : VectorBaseActivity<ActivityTelegramChatBinding>() {

    companion object {
        fun newIntent(context: Context): Intent =
            Intent(context, TelegramStoryViewerActivity::class.java)
    }

    private val stories = listOf(
        StoryPeer("Alice", 0xFFE91E63.toInt(), listOf(
            StoryFrame("Alice", "Hello from vacation! \uD83C\uDF0A", true),
            StoryFrame("Alice", "Day 2 at the beach \uD83C\uDFD6\uFE0F", true)
        )),
        StoryPeer("Bob", 0xFF2196F3.toInt(), listOf(
            StoryFrame("Bob", "New project launch! \uD83D\uDE80", true)
        )),
        StoryPeer("News", 0xFF4CAF50.toInt(), listOf(
            StoryFrame("News", "Breaking: Major update released", false),
            StoryFrame("News", "Community feedback roundup", false)
        )),
        StoryPeer("Team", 0xFFFF9800.toInt(), listOf(
            StoryFrame("Team", "Sprint planning notes", false),
            StoryFrame("Team", "Retrospective highlights", false),
            StoryFrame("Team", "Next quarter goals", false)
        ))
    )

    private var currentPeerIdx = 0
    private var currentFrameIdx = 0
    private val mainHandler = Handler(Looper.getMainLooper())
    private var paused = false
    private var playbackRunnable: Runnable? = null

    data class StoryPeer(val name: String, val color: Int, val frames: List<StoryFrame>)
    data class StoryFrame(val peer: String, val text: String, val hasMedia: Boolean)

    override fun getBinding() = ActivityTelegramChatBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.apply {
            title = "Stories"
            setDisplayHomeAsUpEnabled(true)
        }
        views.telegramMsgInputBar.isVisible = false

        showStory(0, 0)
    }

    private fun showStory(peerIdx: Int, frameIdx: Int) {
        currentPeerIdx = peerIdx.coerceIn(0, stories.lastIndex)
        currentFrameIdx = frameIdx.coerceIn(0, stories[currentPeerIdx].frames.lastIndex)
        val peer = stories[currentPeerIdx]
        val frame = peer.frames[currentFrameIdx]

        views.telegramMsgRecycler.removeAllViews()

        val container = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(16, 24, 16, 16)
        }

        // Segments progress bar (Slider — tdesktop pattern)
        val segments = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL
            setPadding(0, 0, 0, 16)
        }
        peer.frames.forEachIndexed { i, _ ->
            val seg = View(this).apply {
                layoutParams = LinearLayout.LayoutParams(0, 4, 1f).apply {
                    setMargins(2, 0, 2, 0)
                }
                setBackgroundColor(
                    if (i <= currentFrameIdx) 0xFFFFFFFF.toInt()
                    else 0x66FFFFFF.toInt()
                )
            }
            segments.addView(seg)
        }
        container.addView(segments)

        // Header — tdesktop pattern: name + timestamp
        val header = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL
            gravity = android.view.Gravity.CENTER_VERTICAL
            setPadding(0, 0, 0, 16)
        }

        val avatar = View(this).apply {
            layoutParams = LinearLayout.LayoutParams(40, 40).apply { marginEnd = 12 }
            setBackgroundColor(peer.color)
        }
        header.addView(avatar)

        val nameView = TextView(this).apply {
            text = peer.name
            textSize = 16f
            setTextColor(0xFFFFFFFF.toInt())
            layoutParams = LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT, 1f)
        }
        header.addView(nameView)

        val closeBtn = TextView(this).apply {
            text = "✕"
            textSize = 20f
            setTextColor(0xFFFFFFFF.toInt())
            setPadding(16, 8, 0, 8)
            setOnClickListener { finish() }
        }
        header.addView(closeBtn)
        container.addView(header)

        // Story content — media placeholder + text
        val contentArea = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            gravity = android.view.Gravity.CENTER
            layoutParams = LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT, 0, 3f
            )
        }

        val mediaPreview = View(this).apply {
            layoutParams = LinearLayout.LayoutParams(280, 280).apply { bottomMargin = 16 }
            setBackgroundColor(peer.color)
        }
        contentArea.addView(mediaPreview)

        val textView = TextView(this).apply {
            text = frame.text
            textSize = 14f
            setTextColor(0xFFFFFFFF.toInt())
            gravity = android.view.Gravity.CENTER
        }
        contentArea.addView(textView)

        contentArea.setOnClickListener { paused = !paused }
        container.addView(contentArea)

        // Reply area — tdesktop pattern: inline reply input
        val replyBar = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL
            gravity = android.view.Gravity.CENTER_VERTICAL
            setPadding(0, 16, 0, 0)
        }
        val replyInput = android.widget.EditText(this).apply {
            hint = "Reply to story..."
            setHintTextColor(0x88FFFFFF.toInt())
            setTextColor(0xFFFFFFFF.toInt())
            layoutParams = LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT, 1f)
        }
        replyBar.addView(replyInput)
        val sendBtn = TextView(this).apply {
            text = "Send"
            setTextColor(0xFF64B5F6.toInt())
            setPadding(12, 0, 0, 0)
            setOnClickListener {
                val text = replyInput.text.toString().trim()
                if (text.isNotBlank()) {
                    Toast.makeText(context, "Reply sent: $text", Toast.LENGTH_SHORT).show()
                    replyInput.text?.clear()
                }
            }
        }
        replyBar.addView(sendBtn)
        container.addView(replyBar)

        // Reactions bar — tdesktop pattern
        val reactionsBar = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL
            gravity = android.view.Gravity.CENTER
            setPadding(0, 16, 0, 0)
        }
        listOf("\u2764\uFE0F", "\uD83D\uDC4D", "\uD83D\uDD25", "\uD83D\uDE02", "\uD83D\uDE2E", "\uD83D\uDE22").forEach { emoji ->
            val reactBtn = TextView(this).apply {
                text = emoji
                textSize = 24f
                setPadding(8, 4, 8, 4)
                setOnClickListener {
                    Toast.makeText(context, "Reacted $emoji", Toast.LENGTH_SHORT).show()
                }
            }
            reactionsBar.addView(reactBtn)
        }
        container.addView(reactionsBar)

        // Views — tdesktop pattern: "Seen by X, Y and Z others"
        val viewsLabel = TextView(this).apply {
            text = "Seen by Alice, Bob and 12 others"
            textSize = 12f
            setTextColor(0x88FFFFFF.toInt())
            gravity = android.view.Gravity.CENTER
            setPadding(0, 12, 0, 0)
            setOnClickListener {
                AlertDialog.Builder(this@TelegramStoryViewerActivity)
                    .setTitle("Who Viewed")
                    .setMessage("Alice\nBob\nCharlie\nDavid\nEve\n+ 10 more")
                    .setPositiveButton("OK", null)
                    .show()
            }
        }
        container.addView(viewsLabel)

        // Load native stories
        val h = NativeSessionManager.getHandle()
        if (h != 0L) PcpNative.tgGetActiveStories(h, 100)

        views.telegramMsgRecycler.isVisible = false
        views.telegramMsgRecycler.addView(container)
        views.telegramMsgRecycler.isVisible = true
    }

    // Auto-advance — tdesktop Slider pattern with playback
    private fun scheduleNextFrame() {
        playbackRunnable?.let { mainHandler.removeCallbacks(it) }
        if (paused) return

        playbackRunnable = Runnable {
            if (currentFrameIdx + 1 < stories[currentPeerIdx].frames.size) {
                currentFrameIdx++
            } else if (currentPeerIdx + 1 < stories.size) {
                currentPeerIdx++
                currentFrameIdx = 0
            } else {
                finish()
                return@Runnable
            }
            showStory(currentPeerIdx, currentFrameIdx)
            scheduleNextFrame()
        }
        mainHandler.postDelayed(playbackRunnable!!, 5000)
    }

    override fun onResume() {
        super.onResume()
        scheduleNextFrame()
    }

    override fun onPause() {
        super.onPause()
        playbackRunnable?.let { mainHandler.removeCallbacks(it) }
    }
}
