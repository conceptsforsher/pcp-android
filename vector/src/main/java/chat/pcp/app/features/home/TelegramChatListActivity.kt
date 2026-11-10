package chat.pcp.app.features.home

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.Menu
import android.view.View
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import chat.pcp.app.native.PcpNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramListBinding

class TelegramChatListActivity : VectorBaseActivity<ActivityTelegramListBinding>() {

    companion object {
        fun newIntent(context: Context): Intent {
            return Intent(context, TelegramChatListActivity::class.java)
        }
    }

    override fun getBinding() = ActivityTelegramListBinding.inflate(layoutInflater)

    override val rootView: View
        get() = views.root

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.apply {
            title = "Telegram"
            setDisplayHomeAsUpEnabled(true)
        }

        supportFragmentManager.beginTransaction()
            .replace(R.id.telegramChatListContainer, TelegramChatListFragment())
            .commit()
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menu.add("Saved Messages").setOnMenuItemClickListener {
            val h = NativeSessionManager.getHandle()
            val selfId = NativeSessionManager.getUserId().toLongOrNull() ?: 0
            if (selfId > 0) {
                startActivity(TelegramChatActivity.newIntent(this, selfId, "Saved Messages"))
            } else {
                Toast.makeText(this, "User ID not available", Toast.LENGTH_SHORT).show()
            }
            true
        }
        menu.add("Stories").setOnMenuItemClickListener {
            startActivity(TelegramStoryViewerActivity.newIntent(this))
            true
        }
        menu.add("Import Contacts").setOnMenuItemClickListener {
            val input = EditText(this).apply { hint = "Phones: +7900...,+7901..." }
            AlertDialog.Builder(this)
                .setTitle("Import Contacts")
                .setView(input)
                .setPositiveButton("Import") { _, _ ->
                    val phones = input.text.toString().split(",").map { it.trim() }.filter { it.isNotBlank() }
                    if (phones.isNotEmpty()) {
                        val contacts = phones.map { arrayOf(it, it.removePrefix("+").take(10)) }.toTypedArray()
                        PcpNative.tgImportContacts(NativeSessionManager.getHandle(), contacts)
                        Toast.makeText(this, "Importing ${phones.size} contacts...", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
            true
        }
        menu.add("Search Contacts").setOnMenuItemClickListener {
            val input = EditText(this).apply { hint = "Search..." }
            AlertDialog.Builder(this)
                .setTitle("Search Contacts")
                .setView(input)
                .setPositiveButton("Search") { _, _ ->
                    val q = input.text.toString().trim()
                    if (q.isNotBlank()) {
                        PcpNative.tgSearchContacts(NativeSessionManager.getHandle(), q, 100)
                        Toast.makeText(this, "Searching...", Toast.LENGTH_SHORT).show()
                    }
                }
                .setNegativeButton("Cancel", null)
                .show()
            true
        }
        menu.add("New Group").setOnMenuItemClickListener {
            val input = EditText(this).apply { hint = "Group name" }
            AlertDialog.Builder(this)
                .setTitle("New Group Chat")
                .setView(input)
                .setPositiveButton("Create") { _, _ ->
                    val name = input.text.toString().trim().ifEmpty { "New Group" }
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgCreateBasicGroup(h, longArrayOf(), name)
                    Toast.makeText(this, "Creating group: $name", Toast.LENGTH_SHORT).show()
                }
                .setNegativeButton("Cancel", null)
                .show()
            true
        }
        menu.add("New Channel").setOnMenuItemClickListener {
            val input = EditText(this).apply { hint = "Channel name" }
            AlertDialog.Builder(this)
                .setTitle("New Channel")
                .setView(input)
                .setPositiveButton("Create") { _, _ ->
                    val name = input.text.toString().trim().ifEmpty { "New Channel" }
                    val h = NativeSessionManager.getHandle()
                    PcpNative.tgCreateSupergroup(h, name, true, "")
                    Toast.makeText(this, "Creating channel: $name", Toast.LENGTH_SHORT).show()
                }
                .setNegativeButton("Cancel", null)
                .show()
            true
        }
        menu.add("Archived Stories").setOnMenuItemClickListener {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetArchivedStories(h, 0, 100)
            Toast.makeText(this, "Loading archived stories...", Toast.LENGTH_SHORT).show()
            true
        }
        menu.add("Blocked Users").setOnMenuItemClickListener {
            val h = NativeSessionManager.getHandle()
            PcpNative.tgGetBlockedUsers(h, 0, 100)
            Toast.makeText(this, "Loading blocked users...", Toast.LENGTH_SHORT).show()
            true
        }
        return true
    }
}
