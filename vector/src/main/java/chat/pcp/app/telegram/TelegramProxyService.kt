package chat.pcp.app.telegram

import org.json.JSONObject

class TelegramProxyService(private val client: TdLibClient) {

    fun addProxy(
        server: String,
        port: Int,
        enable: Boolean = true,
        proxyType: ProxyType = ProxyType.SOCKS5,
        username: String? = null,
        password: String? = null,
        secret: String? = null
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "addProxy")
            put("server", server)
            put("port", port)
            put("enable", enable)
            put("type", JSONObject().apply {
                put("@type", when (proxyType) {
                    ProxyType.SOCKS5 -> "proxyTypeSocks5"
                    ProxyType.HTTP -> "proxyTypeHttp"
                    ProxyType.MTPROTO -> "proxyTypeMtproto"
                })
                if (proxyType == ProxyType.MTPROTO && secret != null) {
                    put("secret", secret)
                }
            })
            if (username != null) put("username", username)
            if (password != null) put("password", password)
        })
    }

    fun editProxy(
        proxyId: Int,
        server: String,
        port: Int,
        enable: Boolean,
        proxyType: ProxyType = ProxyType.SOCKS5,
        username: String? = null,
        password: String? = null,
        secret: String? = null
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "editProxy")
            put("proxy_id", proxyId)
            put("server", server)
            put("port", port)
            put("enable", enable)
            put("type", JSONObject().apply {
                put("@type", when (proxyType) {
                    ProxyType.SOCKS5 -> "proxyTypeSocks5"
                    ProxyType.HTTP -> "proxyTypeHttp"
                    ProxyType.MTPROTO -> "proxyTypeMtproto"
                })
                if (proxyType == ProxyType.MTPROTO && secret != null) {
                    put("secret", secret)
                }
            })
            if (username != null) put("username", username)
            if (password != null) put("password", password)
        })
    }

    fun enableProxy(proxyId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "enableProxy")
            put("proxy_id", proxyId)
        })
    }

    fun disableProxy() {
        client.sendRequest(JSONObject().apply {
            put("@type", "disableProxy")
        })
    }

    fun removeProxy(proxyId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "removeProxy")
            put("proxy_id", proxyId)
        })
    }

    fun getProxies() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getProxies")
        })
    }

    fun getProxyLink(proxyId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getProxyLink")
            put("proxy_id", proxyId)
        })
    }

    fun pingProxy(proxyId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "pingProxy")
            put("proxy_id", proxyId)
        })
    }

    fun testProxy(
        server: String,
        port: Int,
        proxyType: ProxyType = ProxyType.SOCKS5,
        dcId: Int = 2,
        timeout: Double = 10.0,
        username: String? = null,
        password: String? = null,
        secret: String? = null
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "testProxy")
            put("server", server)
            put("port", port)
            put("type", JSONObject().apply {
                put("@type", when (proxyType) {
                    ProxyType.SOCKS5 -> "proxyTypeSocks5"
                    ProxyType.HTTP -> "proxyTypeHttp"
                    ProxyType.MTPROTO -> "proxyTypeMtproto"
                })
                if (proxyType == ProxyType.MTPROTO && secret != null) {
                    put("secret", secret)
                }
            })
            put("dc_id", dcId)
            put("timeout", timeout)
            if (username != null) put("username", username)
            if (password != null) put("password", password)
        })
    }

    enum class ProxyType { SOCKS5, HTTP, MTPROTO }
}
