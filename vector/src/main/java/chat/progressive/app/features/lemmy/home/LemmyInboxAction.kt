package chat.progressive.app.features.lemmy.home

import im.vector.app.core.platform.VectorViewModelAction

sealed class LemmyInboxAction : VectorViewModelAction {
    object Refresh : LemmyInboxAction()
    data class MarkRead(val item: LemmyInboxItem) : LemmyInboxAction()
    object MarkAllRead : LemmyInboxAction()
}
