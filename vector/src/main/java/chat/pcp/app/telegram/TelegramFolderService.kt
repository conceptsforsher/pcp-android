package chat.pcp.app.telegram

import org.json.JSONArray
import org.json.JSONObject

class TelegramFolderService(private val client: TdLibClient) {

    fun getChatFolder(folderId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatFolder")
            put("chat_folder_id", folderId)
        })
    }

    fun createChatFolder(folder: ChatFolderConfig) {
        client.sendRequest(JSONObject().apply {
            put("@type", "createChatFolder")
            put("folder", JSONObject().apply {
                put("@type", "chatFolder")
                put("title", folder.title)
                put("icon", JSONObject().apply {
                    put("@type", "chatFolderIcon")
                    put("name", folder.iconName)
                })
                put("pinned_chat_ids", JSONArray(folder.pinnedChatIds))
                put("included_chat_ids", JSONArray(folder.includedChatIds))
                put("excluded_chat_ids", JSONArray(folder.excludedChatIds))
                put("exclude_muted", folder.excludeMuted)
                put("exclude_read", folder.excludeRead)
                put("exclude_archived", folder.excludeArchived)
                put("include_contacts", folder.includeContacts)
                put("include_non_contacts", folder.includeNonContacts)
                put("include_bots", folder.includeBots)
                put("include_groups", folder.includeGroups)
                put("include_channels", folder.includeChannels)
            })
        })
    }

    fun editChatFolder(folderId: Int, folder: ChatFolderConfig) {
        client.sendRequest(JSONObject().apply {
            put("@type", "editChatFolder")
            put("chat_folder_id", folderId)
            put("folder", JSONObject().apply {
                put("@type", "chatFolder")
                put("title", folder.title)
                put("icon", JSONObject().apply {
                    put("@type", "chatFolderIcon")
                    put("name", folder.iconName)
                })
                put("pinned_chat_ids", JSONArray(folder.pinnedChatIds))
                put("included_chat_ids", JSONArray(folder.includedChatIds))
                put("excluded_chat_ids", JSONArray(folder.excludedChatIds))
                put("exclude_muted", folder.excludeMuted)
                put("exclude_read", folder.excludeRead)
                put("exclude_archived", folder.excludeArchived)
                put("include_contacts", folder.includeContacts)
                put("include_non_contacts", folder.includeNonContacts)
                put("include_bots", folder.includeBots)
                put("include_groups", folder.includeGroups)
                put("include_channels", folder.includeChannels)
            })
        })
    }

    fun deleteChatFolder(folderId: Int, leaveChatIds: List<Long> = emptyList()) {
        client.sendRequest(JSONObject().apply {
            put("@type", "deleteChatFolder")
            put("chat_folder_id", folderId)
            put("leave_chat_ids", JSONArray(leaveChatIds))
        })
    }

    fun getChatFolderChatsToLeave(folderId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatFolderChatsToLeave")
            put("chat_folder_id", folderId)
        })
    }

    fun getChatFolderDefaultIconName(folder: ChatFolderConfig) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatFolderDefaultIconName")
            put("folder", JSONObject().apply {
                put("title", folder.title)
            })
        })
    }

    fun getChatFolderNewChats(folderId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatFolderNewChats")
            put("chat_folder_id", folderId)
        })
    }

    fun addChatToChatFolder(folderId: Int, chatIds: List<Long>) {
        client.sendRequest(JSONObject().apply {
            put("@type", "addChatFolderToList")
            put("chat_folder_id", folderId)
            put("chat_ids", JSONArray(chatIds))
        })
    }

    fun getChatsForChatFolderInviteLink(folderId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatsForChatFolderInviteLink")
            put("chat_folder_id", folderId)
        })
    }

    fun getChatFolderInviteLinks(folderId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatFolderInviteLinks")
            put("chat_folder_id", folderId)
        })
    }

    fun getRecommendedChatFolders() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getRecommendedChatFolders")
        })
    }
}

data class ChatFolderConfig(
    val title: String,
    val iconName: String = "Custom",
    val pinnedChatIds: List<Long> = emptyList(),
    val includedChatIds: List<Long> = emptyList(),
    val excludedChatIds: List<Long> = emptyList(),
    val excludeMuted: Boolean = false,
    val excludeRead: Boolean = false,
    val excludeArchived: Boolean = true,
    val includeContacts: Boolean = true,
    val includeNonContacts: Boolean = true,
    val includeBots: Boolean = true,
    val includeGroups: Boolean = true,
    val includeChannels: Boolean = true
)
