package chat.pcp.app.features.home

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import chat.pcp.app.irc.IrcSessionHolder
import chat.pcp.app.protocol.IProtocolRoom
import com.google.android.material.bottomsheet.BottomSheetDialogFragment
import dagger.hilt.android.AndroidEntryPoint
import javax.inject.Inject

@AndroidEntryPoint
class IrcChatListBottomSheet : BottomSheetDialogFragment() {

    @Inject lateinit var ircSessionHolder: IrcSessionHolder

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        val recyclerView = RecyclerView(requireContext()).apply {
            layoutManager = LinearLayoutManager(requireContext())
            adapter = IrcChannelAdapter(ircSessionHolder.getExistingSession()?.getChats().orEmpty())
        }
        return recyclerView
    }

    private class IrcChannelAdapter(private val channels: List<IProtocolRoom>) :
        RecyclerView.Adapter<IrcChannelAdapter.ViewHolder>() {

        class ViewHolder(val textView: TextView) : RecyclerView.ViewHolder(textView)

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
            val tv = TextView(parent.context).apply {
                setPadding(48, 24, 48, 24)
                textSize = 16f
            }
            return ViewHolder(tv)
        }

        override fun onBindViewHolder(holder: ViewHolder, position: Int) {
            val channel = channels[position]
            holder.textView.text = "${channel.title} (${channel.protocolType.displayName})"
        }

        override fun getItemCount() = channels.size
    }

    companion object {
        const val TAG = "IRC_CHAT_LIST"
    }
}
