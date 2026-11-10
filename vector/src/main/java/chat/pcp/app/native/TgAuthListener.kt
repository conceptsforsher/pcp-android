package chat.pcp.app.native

interface TgAuthListener {
    fun onAuthStateChanged(type: String, stateJson: String) {}
    fun onConnectionStateChanged(state: String) {}
}
