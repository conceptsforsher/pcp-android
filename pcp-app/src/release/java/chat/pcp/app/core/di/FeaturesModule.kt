/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.core.di

import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import chat.pcp.app.features.DefaultPcpFeatures
import chat.pcp.app.features.DefaultPcpOverrides
import chat.pcp.app.features.PcpFeatures
import chat.pcp.app.features.PcpOverrides

@InstallIn(SingletonComponent::class)
@Module
object FeaturesModule {

    @Provides
    fun providesFeatures(): PcpFeatures {
        return DefaultPcpFeatures()
    }

    @Provides
    fun providesOverrides(): PcpOverrides {
        return DefaultPcpOverrides()
    }
}
