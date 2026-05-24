package chat.progressive.app.features.delta

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import chat.progressive.app.delta.DeltaSessionHolder
import chat.progressive.app.native.ProgressiveNative
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.R
import javax.inject.Inject

@AndroidEntryPoint
class DeltaProfileActivity : AppCompatActivity() {

    @Inject lateinit var deltaSessionHolder: DeltaSessionHolder

    companion object {
        fun newIntent(context: Context): Intent {
            return Intent(context, DeltaProfileActivity::class.java)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_delta_profile)

        val session = deltaSessionHolder.getSession() ?: return

        val displayName = findViewById<EditText>(R.id.profileDisplayName)
        val statusText = findViewById<EditText>(R.id.profileStatus)
        val infoText = ProgressiveNative.dcGetInfo(session.getNativeHandle()) ?: "{}"

        // Parse info JSON to pre-fill fields
        try {
            val info = org.json.JSONObject(infoText)
            displayName.setText(info.optString("displayname", ""))
            statusText.setText(info.optString("selfstatus", ""))
        } catch (_: Exception) {}

        findViewById<android.view.View>(R.id.profileSaveButton).setOnClickListener {
            val name = displayName.text.toString().trim()
            if (name.isNotEmpty()) {
                session.setConfig("displayname", name)
            }
            val status = statusText.text.toString().trim()
            if (status.isNotEmpty()) {
                session.setConfig("selfstatus", status)
            }

            // Update avatar if provided
            val avatarPath = findViewById<EditText>(R.id.profileAvatar).text.toString().trim()
            if (avatarPath.isNotEmpty()) {
                session.setConfig("selfavatar", avatarPath)
            }

            Toast.makeText(this, "Profile saved", Toast.LENGTH_SHORT).show()
            finish()
        }

        findViewById<android.view.View>(R.id.profileAddressBookSync).setOnClickListener {
            syncAddressBook()
        }

        findViewById<android.view.View>(R.id.profileAutoReplyButton).setOnClickListener {
            showAutoReplyDialog(session)
        }
    }

    private fun syncAddressBook() {
        AlertDialog.Builder(this)
            .setTitle("Sync Address Book")
            .setMessage("Add all device contacts as DeltaChat contacts?\n\n" +
                    "If a contact uses DeltaChat on another device, messages will be encrypted automatically.")
            .setPositiveButton("Sync") { _, _ ->
                val contacts = contentResolver.query(
                    android.provider.ContactsContract.CommonDataKinds.Email.CONTENT_URI,
                    arrayOf(android.provider.ContactsContract.CommonDataKinds.Email.ADDRESS,
                        android.provider.ContactsContract.CommonDataKinds.Email.DISPLAY_NAME),
                    null, null, null
                )
                var count = 0
                contacts?.use { cursor ->
                    val addrIdx = cursor.getColumnIndex(android.provider.ContactsContract.CommonDataKinds.Email.ADDRESS)
                    val nameIdx = cursor.getColumnIndex(android.provider.ContactsContract.CommonDataKinds.Email.DISPLAY_NAME)
                    val session = deltaSessionHolder.getSession()
                    while (cursor.moveToNext() && count < 500) {
                        val addr = cursor.getString(addrIdx)
                        val name = cursor.getString(nameIdx) ?: addr
                        session?.createContact(name, addr)
                        count++
                    }
                }
                Toast.makeText(this, "Synced $count contacts", Toast.LENGTH_SHORT).show()
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun showAutoReplyDialog(session: chat.progressive.app.delta.DeltaSession) {
        val input = EditText(this).apply {
            setText(session.getConfig("auto_reply") ?: "")
            hint = "Auto-reply message"
        }
        AlertDialog.Builder(this)
            .setTitle("Auto-Reply")
            .setView(input)
            .setPositiveButton("Save") { _, _ ->
                session.setConfig("auto_reply", input.text.toString())
                Toast.makeText(this, "Auto-reply saved", Toast.LENGTH_SHORT).show()
            }
            .setNeutralButton("Disable") { _, _ ->
                session.setConfig("auto_reply", "")
                Toast.makeText(this, "Auto-reply disabled", Toast.LENGTH_SHORT).show()
            }
            .setNegativeButton("Cancel", null)
            .show()
    }
}
