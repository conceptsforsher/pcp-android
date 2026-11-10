package chat.pcp.app.xmpp

import chat.pcp.app.protocol.ProtocolManager
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object XmppModule {

    @Provides
    @Singleton
    fun provideXmppSessionHolder(): XmppSessionHolder = XmppSessionHolder()

    @Provides
    @Singleton
    fun provideProtocolManager(xmppHolder: XmppSessionHolder): ProtocolManager {
        return ProtocolManager(xmppHolder)
    }
}
