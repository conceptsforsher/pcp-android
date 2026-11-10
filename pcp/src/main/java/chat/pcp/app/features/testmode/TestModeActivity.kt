package chat.pcp.app.features.testmode

import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import chat.pcp.app.R
import org.json.JSONArray
import org.json.JSONObject

class TestModeActivity : AppCompatActivity() {

    private lateinit var roomList: RecyclerView
    private lateinit var messageList: RecyclerView
    private lateinit var searchInput: EditText
    private lateinit var statusText: TextView
    private lateinit var roomAdapter: RoomAdapter
    private lateinit var messageAdapter: MessageAdapter

    private val testRooms = """[{"roomId":"!test1:localhost","name":"General Chat","topic":"Welcome to PCP — test mode","memberCount":5,"unreadCount":3,"isEncrypted":false,"lastMessage":"Hey everyone! Welcome to the test room 👋","lastTimestamp":"2026-05-30T10:00:00Z"},{"roomId":"!test2:localhost","name":"Encrypted Room 🔒","topic":"This room is end-to-end encrypted","memberCount":3,"unreadCount":0,"isEncrypted":true,"lastMessage":"This room is encrypted. Your messages are safe.","lastTimestamp":"2026-05-30T09:30:00Z"},{"roomId":"!test3:localhost","name":"Bot Testing 🤖","topic":"Test bots and integrations here","memberCount":4,"unreadCount":7,"isEncrypted":false,"lastMessage":"!help — try bot commands here","lastTimestamp":"2026-05-30T11:00:00Z"},{"roomId":"!test4:localhost","name":"Announcements 📢","topic":"Important updates and news","memberCount":10,"unreadCount":0,"isEncrypted":false,"lastMessage":"Server maintenance scheduled for tonight","lastTimestamp":"2026-05-29T18:00:00Z"},{"roomId":"!test5:localhost","name":"Random / Off-Topic","topic":"Anything goes","memberCount":8,"unreadCount":2,"isEncrypted":false,"lastMessage":"Check out this cool project","lastTimestamp":"2026-05-30T08:00:00Z"}]"""

    private val testMessages = """{"!test1:localhost":[{"eventId":"${'$'}ev1","senderId":"@alice:localhost","senderName":"Alice","body":"Hey everyone! Welcome to the test room 👋","timestamp":"10:00","originServerTs":1748601600,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev2","senderId":"@bob:localhost","senderName":"Bob","body":"Hi Alice! This test mode is really useful for debugging.","timestamp":"10:01","originServerTs":1748601660,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev3","senderId":"@carol:localhost","senderName":"Carol","body":"Agreed! I can test all the features without an account.","timestamp":"10:02","originServerTs":1748601720,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev4","senderId":"@me:localhost","senderName":"You","body":"Testing message composition...","timestamp":"10:03","originServerTs":1748601780,"isEncrypted":false,"sentByMe":true,"msgType":"m.text"},{"eventId":"${'$'}ev5","senderId":"@alice:localhost","senderName":"Alice","body":"Here is a longer message to test text rendering with different lengths. It should wrap properly in the bubble.","timestamp":"10:04","originServerTs":1748601840,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev6","senderId":"@bob:localhost","senderName":"Bob","body":"Markdown test: bold, italic, code, strikethrough","timestamp":"10:05","originServerTs":1748601900,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"}],"!test2:localhost":[{"eventId":"${'$'}ev8","senderId":"@dave:localhost","senderName":"Dave","body":"Welcome to the encrypted room 🔒","timestamp":"09:30","originServerTs":1748599800,"isEncrypted":true,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev9","senderId":"@eve:localhost","senderName":"Eve","body":"All messages here are end-to-end encrypted","timestamp":"09:31","originServerTs":1748599860,"isEncrypted":true,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev10","senderId":"@me:localhost","senderName":"You","body":"Testing encrypted message sending","timestamp":"09:32","originServerTs":1748599920,"isEncrypted":true,"sentByMe":true,"msgType":"m.text"}],"!test3:localhost":[{"eventId":"${'$'}ev12","senderId":"@frank:localhost","senderName":"Frank","body":"!help — try bot commands","timestamp":"11:00","originServerTs":1748605200,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev13","senderId":"@bot:localhost","senderName":"TestBot","body":"Available commands: !help, !time, !weather, !flip, !roll","timestamp":"11:00","originServerTs":1748605201,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev14","senderId":"@me:localhost","senderName":"You","body":"!time","timestamp":"11:01","originServerTs":1748605260,"isEncrypted":false,"sentByMe":true,"msgType":"m.text"},{"eventId":"${'$'}ev15","senderId":"@bot:localhost","senderName":"TestBot","body":"Current time: 2026-05-30 11:01 UTC","timestamp":"11:01","originServerTs":1748605261,"isEncrypted":false,"sentByMe":false,"msgType":"m.notice"}],"!test4:localhost":[{"eventId":"${'$'}ev16","senderId":"@admin:localhost","senderName":"Admin","body":"Server maintenance scheduled for tonight at 02:00 UTC","timestamp":"18:00","originServerTs":1748548800,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev17","senderId":"@admin:localhost","senderName":"Admin","body":"Expected downtime: 15 minutes","timestamp":"18:01","originServerTs":1748548860,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"}],"!test5:localhost":[{"eventId":"${'$'}ev18","senderId":"@grace:localhost","senderName":"Grace","body":"Check out this cool project: https://pcp.chat","timestamp":"08:00","originServerTs":1748594400,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev19","senderId":"@heidi:localhost","senderName":"Heidi","body":"Nice! The C++ native core is really fast.","timestamp":"08:05","originServerTs":1748594700,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"},{"eventId":"${'$'}ev20","senderId":"@ivan:localhost","senderName":"Ivan","body":"Has anyone tried the new encrypted search feature?","timestamp":"08:10","originServerTs":1748595000,"isEncrypted":false,"sentByMe":false,"msgType":"m.text"}]}"""

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_test_mode)

        roomList = findViewById(R.id.testRoomList)
        messageList = findViewById(R.id.testMessageList)
        searchInput = findViewById(R.id.testSearchInput)
        statusText = findViewById(R.id.testStatus)

        roomAdapter = RoomAdapter { roomId -> loadMessages(roomId) }
        messageAdapter = MessageAdapter()

        roomList.layoutManager = LinearLayoutManager(this)
        roomList.adapter = roomAdapter
        messageList.layoutManager = LinearLayoutManager(this)
        messageList.adapter = messageAdapter

        findViewById<View>(R.id.testSearchBtn).setOnClickListener { doSearch() }

        loadRooms()
    }

    private fun loadRooms() {
        try {
            val arr = JSONArray(testRooms)
            val rooms = (0 until arr.length()).map { arr.getJSONObject(it) }
            roomAdapter.submitList(rooms)
            statusText.text = "${rooms.size} test rooms loaded"
        } catch (e: Exception) {
            statusText.text = "Error: ${e.message}"
        }
    }

    private fun loadMessages(roomId: String) {
        try {
            val allMessages = JSONObject(testMessages)
            val arr = allMessages.optJSONArray(roomId) ?: JSONArray()
            val msgs = (0 until arr.length()).map { arr.getJSONObject(it) }
            messageAdapter.submitList(msgs)
            messageList.visibility = View.VISIBLE
            statusText.text = "${msgs.size} messages"
        } catch (e: Exception) {
            statusText.text = "Error: ${e.message}"
        }
    }

    private fun doSearch() {
        val term = searchInput.text.toString().lowercase()
        if (term.isBlank()) return
        try {
            val allMessages = JSONObject(testMessages)
            val results = mutableListOf<JSONObject>()
            for (key in allMessages.keys()) {
                val arr = allMessages.getJSONArray(key)
                for (i in 0 until arr.length()) {
                    val m = arr.getJSONObject(i)
                    if (m.getString("body").lowercase().contains(term) ||
                        m.getString("senderName").lowercase().contains(term)) {
                        m.put("roomId", key)
                        results.add(m)
                    }
                }
            }
            messageAdapter.submitList(results)
            statusText.text = "Found ${results.size} messages for '$term'"
        } catch (e: Exception) {
            statusText.text = "Error: ${e.message}"
        }
    }
}

data class RoomItem(val id: String, val name: String, val topic: String, val unread: Int, val encrypted: Boolean)

class RoomAdapter(private val onClick: (String) -> Unit) : RecyclerView.Adapter<RoomAdapter.VH>() {
    private var items = listOf<JSONObject>()

    fun submitList(list: List<JSONObject>) { items = list; notifyDataSetChanged() }

    @Suppress("DEPRECATION")
    override fun onCreateViewHolder(parent: android.view.ViewGroup, viewType: Int): VH {
        val tv = android.widget.TextView(parent.context).apply {
            setPadding(48, 24, 48, 24); textSize = 16f
        }
        return VH(tv)
    }

    @Suppress("DEPRECATION")
    override fun onBindViewHolder(holder: VH, pos: Int) {
        val r = items[pos]
        val enc = if (r.optBoolean("isEncrypted")) " 🔒" else ""
        holder.v.text = "${r.getString("name")}$enc\n  ${r.getString("lastMessage")}"
        holder.v.setOnClickListener { onClick(r.getString("roomId")) }
    }

    override fun getItemCount() = items.size
    class VH(val v: TextView) : RecyclerView.ViewHolder(v)
}

class MessageAdapter : RecyclerView.Adapter<MessageAdapter.VH>() {
    private var items = listOf<JSONObject>()

    fun submitList(list: List<JSONObject>) { items = list; notifyDataSetChanged() }

    override fun onCreateViewHolder(parent: android.view.ViewGroup, viewType: Int): VH {
        val tv = android.widget.TextView(parent.context).apply {
            setPadding(32, 16, 32, 16); textSize = 14f
        }
        return VH(tv)
    }

    override fun onBindViewHolder(holder: VH, pos: Int) {
        val m = items[pos]
        holder.v.text = "${m.getString("senderName")}: ${m.getString("body")}"
    }

    override fun getItemCount() = items.size
    class VH(val v: TextView) : RecyclerView.ViewHolder(v)
}
