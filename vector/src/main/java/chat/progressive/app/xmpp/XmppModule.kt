package chat.progressive.app.xmpp

import chat.progressive.app.protocol.ProtocolManager
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
