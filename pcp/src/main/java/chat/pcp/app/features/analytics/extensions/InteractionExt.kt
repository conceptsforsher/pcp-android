/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.analytics.extensions

import im.vector.app.features.analytics.plan.Interaction

fun Interaction.Name.toAnalyticsInteraction(interactionType: Interaction.InteractionType = Interaction.InteractionType.Touch) =
        Interaction(
                name = this,
                interactionType = interactionType
        )
