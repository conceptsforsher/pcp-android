package chat.pcp.app.lemmy

import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object LemmyModule {

    @Provides
    @Singleton
    fun provideLemmySessionHolder(): LemmySessionHolder {
        return LemmySessionHolder()
    }
}
