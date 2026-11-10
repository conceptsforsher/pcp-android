package chat.pcp.app.features.lemmy.home

import im.vector.app.core.platform.VectorViewModelAction

sealed class LemmyPostDetailAction : VectorViewModelAction {
    object Refresh : LemmyPostDetailAction()
    data class VotePost(val score: Int) : LemmyPostDetailAction()
    data class VoteComment(val commentId: Int, val score: Int) : LemmyPostDetailAction()
    data class SetReplyText(val text: String) : LemmyPostDetailAction()
    object SendReply : LemmyPostDetailAction()
    data class ReplyToComment(val commentId: Int) : LemmyPostDetailAction()
}
