/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.debug.di

import android.content.Context
import dagger.Binds
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import chat.pcp.app.features.DefaultPcpFeatures
import chat.pcp.app.features.DefaultPcpOverrides
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.PcpOverrides
import chat.pcp.app.features.debug.features.DebugPcpFeatures
import chat.pcp.app.features.debug.features.DebugPcpOverrides

@InstallIn(SingletonComponent::class)
@Module
interface FeaturesModule {

    @Binds
    fun bindFeatures(debugFeatures: DebugPcpFeatures): PcpFeatures

    @Binds
    fun bindOverrides(debugOverrides: DebugPcpOverrides): PcpOverrides

    companion object {

        @Provides
        fun providesDefaultPcpFeatures(): DefaultPcpFeatures {
            return DefaultPcpFeatures()
        }

        @Provides
        fun providesDebugPcpFeatures(context: Context, defaultPcpFeatures: DefaultPcpFeatures): DebugPcpFeatures {
            return DebugPcpFeatures(context, defaultPcpFeatures)
        }

        @Provides
        fun providesDefaultPcpOverrides(): DefaultPcpOverrides {
            return DefaultPcpOverrides()
        }

        @Provides
        fun providesDebugPcpOverrides(context: Context): DebugPcpOverrides {
            return DebugPcpOverrides(context)
        }
    }
}
