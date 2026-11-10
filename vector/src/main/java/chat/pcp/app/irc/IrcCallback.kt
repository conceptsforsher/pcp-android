package chat.pcp.app.irc

interface IrcCallback {
    fun onIrcMessage(clientId: Long, json: String)
    fun onIrcState(clientId: Long, state: Int)
    fun onIrcError(clientId: Long, code: Int, message: String)
}
