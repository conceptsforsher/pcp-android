package chat.pcp.app.features.delta

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import chat.pcp.app.delta.DeltaContact
import chat.pcp.app.delta.DeltaSessionHolder
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.R
import javax.inject.Inject

@AndroidEntryPoint
class DeltaContactsActivity : AppCompatActivity() {

    @Inject lateinit var deltaSessionHolder: DeltaSessionHolder

    private lateinit var list: RecyclerView
    private lateinit var adapter: ContactAdapter

    companion object {
        fun newIntent(context: Context): Intent {
            return Intent(context, DeltaContactsActivity::class.java)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_delta_contacts)

        list = findViewById(R.id.deltaContactList)
        list.layoutManager = LinearLayoutManager(this)
        adapter = ContactAdapter(
            onContactClick = { contact ->
                showContactDialog(contact)
            },
            onBlockClick = { contact ->
                val session = deltaSessionHolder.getSession() ?: return@ContactAdapter
                session.blockContact(contact.id, true)
                Toast.makeText(this, "Blocked ${contact.name}", Toast.LENGTH_SHORT).show()
            }
        )
        list.adapter = adapter

        findViewById<View>(R.id.deltaAddContactButton).setOnClickListener {
            showAddContactDialog()
        }

        loadContacts()
    }

    private fun loadContacts() {
        val session = deltaSessionHolder.getSession() ?: return
        adapter.setContacts(session.getContacts())
    }

    private fun showContactDialog(contact: DeltaContact) {
        val info = deltaSessionHolder.getSession()?.getContactEncryptionInfo(contact.id) ?: ""
        AlertDialog.Builder(this)
            .setTitle(contact.name)
            .setMessage("Address: ${contact.address}\nEncryption: ${info.ifEmpty { "Standard" }}")
            .setPositiveButton("Chat") { _, _ ->
                // TODO: find or create 1:1 chat with this contact
            }
            .setNeutralButton("Block") { _, _ ->
                deltaSessionHolder.getSession()?.blockContact(contact.id, true)
            }
            .setNegativeButton("Close", null)
            .show()
    }

    private fun showAddContactDialog() {
        val input = android.widget.FrameLayout(this)
        val emailField = android.widget.EditText(this).apply {
            hint = "Email address"
            inputType = android.text.InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS
        }
        val nameField = android.widget.EditText(this).apply {
            hint = "Name (optional)"
        }
        val layout = android.widget.LinearLayout(this).apply {
            orientation = android.widget.LinearLayout.VERTICAL
            addView(nameField)
            addView(emailField)
            (emailField.layoutParams as android.widget.LinearLayout.LayoutParams).topMargin = 16
        }
        input.addView(layout)

        AlertDialog.Builder(this)
            .setTitle("Add Contact")
            .setView(input)
            .setPositiveButton("Add") { _, _ ->
                val name = nameField.text.toString().trim()
                val addr = emailField.text.toString().trim()
                if (addr.isNotEmpty()) {
                    deltaSessionHolder.getSession()?.createContact(name.ifEmpty { addr }, addr)
                    loadContacts()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private class ContactAdapter(
        private val onContactClick: (DeltaContact) -> Unit,
        private val onBlockClick: (DeltaContact) -> Unit
    ) : RecyclerView.Adapter<ContactAdapter.ViewHolder>() {

        private var contacts = listOf<DeltaContact>()

        fun setContacts(c: List<DeltaContact>) {
            contacts = c
            notifyDataSetChanged()
        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
            val view = LayoutInflater.from(parent.context)
                .inflate(R.layout.item_delta_contact, parent, false)
            return ViewHolder(view)
        }

        override fun onBindViewHolder(holder: ViewHolder, position: Int) {
            val c = contacts[position]
            holder.name.text = c.name.ifEmpty { c.address }
            holder.addr.text = c.address
            holder.itemView.setOnClickListener { onContactClick(c) }
            holder.block.setOnClickListener { onBlockClick(c) }
        }

        override fun getItemCount() = contacts.size

        class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {
            val name: TextView = view.findViewById(R.id.contactName)
            val addr: TextView = view.findViewById(R.id.contactAddr)
            val block: TextView = view.findViewById(R.id.contactBlock)
        }
    }
}
