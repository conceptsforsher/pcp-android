package chat.pcp.app.telegram

import org.json.JSONObject

class TelegramFileService(private val client: TdLibClient) {

    fun downloadFile(fileId: Int, priority: Int = 1, offset: Long = 0, limit: Long = 0, synchronous: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "downloadFile")
            put("file_id", fileId)
            put("priority", priority)
            put("offset", offset)
            put("limit", limit)
            put("synchronous", synchronous)
        })
    }

    fun cancelDownloadFile(fileId: Int, onlyIfPending: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "cancelDownloadFile")
            put("file_id", fileId)
            put("only_if_pending", onlyIfPending)
        })
    }

    fun getFile(fileId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getFile")
            put("file_id", fileId)
        })
    }

    fun getRemoteFile(remoteFileId: String, fileType: String? = null) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getRemoteFile")
            put("remote_file_id", remoteFileId)
            if (fileType != null) {
                put("file_type", JSONObject().apply { put("@type", fileType) })
            }
        })
    }

    fun getFileDownloadedPrefixSize(fileId: Int, offset: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getFileDownloadedPrefixSize")
            put("file_id", fileId)
            put("offset", offset)
        })
    }

    fun getFileMimeType(fileName: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getFileMimeType")
            put("file_name", fileName)
        })
    }

    fun getFileExtension(mimeType: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getFileExtension")
            put("mime_type", mimeType)
        })
    }

    fun uploadFile(filePath: String, fileType: String, priority: Int = 1) {
        client.sendRequest(JSONObject().apply {
            put("@type", "uploadFile")
            put("file", JSONObject().apply {
                put("@type", "inputFileLocal")
                put("path", filePath)
            })
            put("file_type", JSONObject().apply { put("@type", fileType) })
            put("priority", priority)
        })
    }

    fun cancelUploadFile(fileId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "cancelUploadFile")
            put("file_id", fileId)
        })
    }

    fun deleteFile(fileId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "deleteFile")
            put("file_id", fileId)
        })
    }

    fun optimizeStorage(
        size: Long,
        ttl: Int = 0,
        count: Int = 0,
        immunityDelay: Int = 0,
        fileTypes: List<String>? = null,
        chatIds: List<Long>? = null,
        excludeChatIds: List<Long>? = null,
        returnDeletedFileStatistics: Boolean = false,
        chunkSize: Int = 0
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "optimizeStorage")
            put("size", size)
            put("ttl", ttl)
            put("count", count)
            put("immunity_delay", immunityDelay)
            if (fileTypes != null) {
                put("file_types", org.json.JSONArray(fileTypes.map { type ->
                    JSONObject().apply { put("@type", type) }
                }))
            }
            if (chatIds != null) {
                put("chat_ids", org.json.JSONArray(chatIds))
            }
            if (excludeChatIds != null) {
                put("exclude_chat_ids", org.json.JSONArray(excludeChatIds))
            }
            put("return_deleted_file_statistics", returnDeletedFileStatistics)
            if (chunkSize > 0) put("chunk_size", chunkSize)
        })
    }
}
