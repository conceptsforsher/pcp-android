package chat.pcp.app.telegram

import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.components.SingletonComponent
import android.content.Context
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object TelegramModule {

    @Provides
    @Singleton
    fun provideTelegramSessionHolder(
        @ApplicationContext context: Context
    ): TelegramSessionHolder {
        return TelegramSessionHolder(context)
    }
}
