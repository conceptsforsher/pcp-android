package chat.pcp.app.delta

import android.content.Context
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object DeltaModule {

    @Provides
    @Singleton
    fun provideDeltaSessionHolder(context: Context): DeltaSessionHolder {
        return DeltaSessionHolder(context)
    }
}
