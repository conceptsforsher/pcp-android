package chat.pcp.app.features.lemmy.home

import im.vector.app.core.platform.VectorViewModelAction

sealed class LemmyHomeAction : VectorViewModelAction {
    object Refresh : LemmyHomeAction()
    data class SelectPost(val post: LemmyPostItem) : LemmyHomeAction()
    data class SelectCommunity(val community: LemmyCommunityItem) : LemmyHomeAction()
    data class VotePost(val postId: Int, val score: Int) : LemmyHomeAction()
    data class SwitchTab(val tab: Int) : LemmyHomeAction()
}
