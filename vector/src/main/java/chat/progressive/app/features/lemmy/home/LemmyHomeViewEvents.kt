package chat.progressive.app.features.lemmy.home

import im.vector.app.core.platform.VectorViewEvents

sealed class LemmyHomeViewEvents : VectorViewEvents {
    data class Failure(val throwable: Throwable) : LemmyHomeViewEvents()
    data class OpenPost(val post: LemmyPostItem) : LemmyHomeViewEvents()
    data class OpenCommunity(val community: LemmyCommunityItem) : LemmyHomeViewEvents()
}
