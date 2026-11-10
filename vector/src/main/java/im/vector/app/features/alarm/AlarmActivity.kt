/*
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP-Commercial
 */

package im.vector.app.features.alarm

import android.content.Context
import android.content.Intent
import android.media.MediaPlayer
import android.media.RingtoneManager
import android.os.Build
import android.os.Bundle
import android.os.VibrationEffect
import android.os.Vibrator
import android.os.VibratorManager
import android.speech.tts.TextToSpeech
import android.view.View
import android.view.WindowManager
import android.widget.TextView
import android.widget.Button
import androidx.core.content.getSystemService
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityAlarmBinding
import chat.pcp.app.native.PcpNative
import java.util.Locale
import java.util.Timer
import java.util.TimerTask

@AndroidEntryPoint
class AlarmActivity : VectorBaseActivity<ActivityAlarmBinding>() {

    private var mediaPlayer: MediaPlayer? = null
    private var vibrator: Vibrator? = null
    private var tts: TextToSpeech? = null
    private var alarmId: String = ""
    private var alarmNote: String = ""
    private var ttsDelay: Int = 5
    private var ttsTimer: Timer? = null

    override fun getBinding() = ActivityAlarmBinding.inflate(layoutInflater)

    override fun getCoordinatorLayout() = views.coordinatorLayout

    override val rootView: View
        get() = views.coordinatorLayout

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Show over lock screen
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
            setShowWhenLocked(true)
            setTurnScreenOn(true)
        }
        window.addFlags(
            WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON or
            WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD or
            WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED or
            WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON
        )

        alarmId = intent.getStringExtra("ALARM_ID") ?: ""
        alarmNote = intent.getStringExtra("ALARM_NOTE") ?: "Alarm"
        ttsDelay = intent.getIntExtra("ALARM_TTS_DELAY", 5)

        setupUi()
        startRingtone()
        startVibration()
        scheduleTts()
    }

    private fun setupUi() {
        views.alarmNoteText.text = alarmNote

        views.alarmDismissButton.setOnClickListener {
            dismissAlarm()
        }

        views.alarmSnoozeButton.setOnClickListener {
            snoozeAlarm(10)
        }

        // Optional: weather/pre-action info
        val preAction = intent.getStringExtra("ALARM_PRE_ACTION")
        if (!preAction.isNullOrBlank()) {
            views.alarmPreActionText.visibility = View.VISIBLE
            views.alarmPreActionText.text = preAction
        }
    }

    private fun startRingtone() {
        try {
            val uri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_ALARM)
            mediaPlayer = MediaPlayer().apply {
                setDataSource(this@AlarmActivity, uri)
                isLooping = true
                setAudioAttributes(
                    android.media.AudioAttributes.Builder()
                        .setUsage(android.media.AudioAttributes.USAGE_ALARM)
                        .setContentType(android.media.AudioAttributes.CONTENT_TYPE_SONIFICATION)
                        .build()
                )
                prepare()
                start()
            }
        } catch (_: Exception) { }
    }

    private fun startVibration() {
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                val vm = getSystemService<VibratorManager>()
                vibrator = vm?.defaultVibrator
            } else {
                @Suppress("DEPRECATION")
                vibrator = getSystemService(Context.VIBRATOR_SERVICE) as? Vibrator
            }
            val pattern = longArrayOf(0, 1000, 500, 1000, 500, 1000)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                vibrator?.vibrate(VibrationEffect.createWaveform(pattern, 0))
            } else {
                @Suppress("DEPRECATION")
                vibrator?.vibrate(pattern, 0)
            }
        } catch (_: Exception) { }
    }

    private fun scheduleTts() {
        if (ttsDelay <= 0) return
        ttsTimer = Timer()
        ttsTimer?.schedule(object : TimerTask() {
            override fun run() {
                runOnUiThread { startTts() }
            }
        }, ttsDelay * 1000L)
    }

    private fun startTts() {
        tts = TextToSpeech(this) { status ->
            if (status == TextToSpeech.SUCCESS) {
                tts?.language = Locale.getDefault()
                tts?.speak(alarmNote, TextToSpeech.QUEUE_FLUSH, null, "ALARM_TTS_${alarmId}")
            }
        }
    }

    private fun snoozeAlarm(minutes: Int) {
        try {
            PcpNative.ensureLoaded()
            PcpNative.nativeAlarmSnooze(alarmId, minutes)
        } catch (_: Exception) { }
        stopAndFinish()
    }

    private fun dismissAlarm() {
        try {
            PcpNative.ensureLoaded()
            PcpNative.nativeAlarmDismiss(alarmId)
        } catch (_: Exception) { }
        stopAndFinish()
    }

    private fun stopAndFinish() {
        ttsTimer?.cancel()
        tts?.stop()
        tts?.shutdown()
        mediaPlayer?.stop()
        mediaPlayer?.release()
        vibrator?.cancel()
        finish()
    }

    override fun onDestroy() {
        stopAndFinish()
        super.onDestroy()
    }

    override fun onBackPressed() {
        snoozeAlarm(5)
    }

    companion object {
        fun newIntent(
            context: Context,
            alarmId: String,
            alarmNote: String,
            ttsDelay: Int = 5,
            preActionText: String? = null
        ): Intent {
            return Intent(context, AlarmActivity::class.java).apply {
                putExtra("ALARM_ID", alarmId)
                putExtra("ALARM_NOTE", alarmNote)
                putExtra("ALARM_TTS_DELAY", ttsDelay)
                putExtra("ALARM_PRE_ACTION", preActionText)
                flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TOP
            }
        }
    }
}
