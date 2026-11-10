/*
 * Copyright 2022-2024 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.location.preview

import android.os.Bundle
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.lifecycle.lifecycleScope
import com.airbnb.mvrx.args
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import dagger.hilt.android.AndroidEntryPoint
import chat.pcp.app.R
import chat.pcp.app.core.platform.PcpFragment
import chat.pcp.app.core.platform.PcpMenuProvider
import chat.pcp.app.core.utils.PERMISSIONS_FOR_FOREGROUND_LOCATION_SHARING
import chat.pcp.app.core.utils.checkPermissions
import chat.pcp.app.core.utils.onPermissionDeniedDialog
import chat.pcp.app.core.utils.openLocation
import chat.pcp.app.core.utils.registerForPermissionsResult
import chat.pcp.app.databinding.FragmentLocationPreviewBinding
import chat.pcp.app.features.location.DEFAULT_PIN_ID
import chat.pcp.app.features.location.LocationSharingArgs
import chat.pcp.app.features.location.MapState
import chat.pcp.app.features.location.UrlMapProvider
import chat.pcp.app.features.location.showUserLocationNotAvailableErrorDialog
import chat.pcp.lib.strings.CommonStrings
import kotlinx.coroutines.launch
import org.maplibre.android.maps.MapView
import java.lang.ref.WeakReference
import javax.inject.Inject

/**
 * Screen displaying the expanded map of a static location share.
 */
@AndroidEntryPoint
class LocationPreviewFragment :
        PcpFragment<FragmentLocationPreviewBinding>(),
        PcpMenuProvider {

    @Inject lateinit var urlMapProvider: UrlMapProvider

    private val args: LocationSharingArgs by args()

    private val viewModel: LocationPreviewViewModel by fragmentViewModel()

    // Keep a ref to handle properly the onDestroy callback
    private var mapView: WeakReference<MapView>? = null

    private var mapLoadingErrorListener: MapView.OnDidFailLoadingMapListener? = null

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentLocationPreviewBinding {
        return FragmentLocationPreviewBinding.inflate(layoutInflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        mapView = WeakReference(views.mapView)
        mapLoadingErrorListener = MapView.OnDidFailLoadingMapListener {
            viewModel.handle(LocationPreviewAction.ShowMapLoadingError)
        }.also { views.mapView.addOnDidFailLoadingMapListener(it) }
        views.mapView.onCreate(savedInstanceState)

        viewLifecycleOwner.lifecycleScope.launch {
            views.mapView.initialize(urlMapProvider.getMapUrl())
        }

        observeViewEvents()
        initLocateButton()
    }

    private fun observeViewEvents() {
        viewModel.observeViewEvents {
            when (it) {
                LocationPreviewViewEvents.UserLocationNotAvailableError -> handleUserLocationNotAvailableError()
                is LocationPreviewViewEvents.ZoomToUserLocation -> handleZoomToUserLocationEvent(it)
            }
        }
    }

    private fun handleUserLocationNotAvailableError() {
        showUserLocationNotAvailableErrorDialog {
            // do nothing
        }
    }

    private fun handleZoomToUserLocationEvent(event: LocationPreviewViewEvents.ZoomToUserLocation) {
        views.mapView.zoomToLocation(event.userLocation)
    }

    override fun onDestroyView() {
        mapLoadingErrorListener?.let { mapView?.get()?.removeOnDidFailLoadingMapListener(it) }
        mapLoadingErrorListener = null
        super.onDestroyView()
    }

    override fun onResume() {
        super.onResume()
        views.mapView.onResume()
    }

    override fun onPause() {
        views.mapView.onPause()
        super.onPause()
    }

    override fun onLowMemory() {
        views.mapView.onLowMemory()
        super.onLowMemory()
    }

    override fun onStart() {
        super.onStart()
        views.mapView.onStart()
    }

    override fun onStop() {
        views.mapView.onStop()
        super.onStop()
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        views.mapView.onSaveInstanceState(outState)
    }

    override fun onDestroy() {
        mapView?.get()?.onDestroy()
        mapView?.clear()
        super.onDestroy()
    }

    override fun invalidate() = withState(viewModel) { state ->
        views.mapPreviewLoadingError.isVisible = state.loadingMapHasFailed
        if (state.isLoadingUserLocation) {
            showLoadingDialog()
        } else {
            dismissLoadingDialog()
        }
        updateMap(state)
    }

    private fun updateMap(viewState: LocationPreviewViewState) {
        views.mapView.render(
                MapState(
                        zoomOnlyOnce = true,
                        pinLocationData = viewState.pinLocationData,
                        pinId = viewState.pinUserId ?: DEFAULT_PIN_ID,
                        pinDrawable = viewState.pinDrawable,
                        userLocationData = viewState.lastKnownUserLocation,
                )
        )
    }

    override fun getMenuRes() = R.menu.menu_location_preview

    override fun handleMenuItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.share_external -> {
                onShareLocationExternal()
                true
            }
            else -> false
        }
    }

    private fun onShareLocationExternal() {
        val location = args.initialLocationData ?: return
        openLocation(requireActivity(), location.latitude, location.longitude)
    }

    private fun initLocateButton() {
        views.mapView.locateButton.setOnClickListener {
            if (checkPermissions(PERMISSIONS_FOR_FOREGROUND_LOCATION_SHARING, requireActivity(), foregroundLocationResultLauncher)) {
                zoomToUserLocation()
            }
        }
    }

    private fun zoomToUserLocation() {
        viewModel.handle(LocationPreviewAction.ZoomToUserLocation)
    }

    private val foregroundLocationResultLauncher = registerForPermissionsResult { allGranted, deniedPermanently ->
        if (allGranted) {
            zoomToUserLocation()
        } else if (deniedPermanently) {
            activity?.onPermissionDeniedDialog(CommonStrings.denied_permission_generic)
        }
    }
}
