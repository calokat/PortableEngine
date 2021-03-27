#include "XRAPI.h"
#include <iostream>
#include <assert.h>
XRAPI::XRAPI(IPlatform* plat, IGraphicsAPI* graph) : platform(plat), graphics(graph)
{
	Init();
	InitializeXRSystem();
	InitializeXRSession();
	CreateSwapchains();
}

XrResult XRAPI::Init()
{
	XrResult res;
	uint32_t nApiLayerProps;
	res = xrEnumerateApiLayerProperties(0, &nApiLayerProps, nullptr);
	std::vector<XrApiLayerProperties> layerProps(nApiLayerProps);
	xrEnumerateApiLayerProperties(nApiLayerProps, &nApiLayerProps, layerProps.data());
	std::cout << "Number of Api Layer Properties: " << nApiLayerProps << std::endl;
	for (int i = 0; i < nApiLayerProps; ++i)
	{
		std::cout << layerProps[i].layerName << std::endl;
	}

	uint32_t nInstanceExtensionProps;
	xrEnumerateInstanceExtensionProperties(nullptr, 0, &nInstanceExtensionProps, nullptr);
	std::vector<XrExtensionProperties> extensionProps(nInstanceExtensionProps, { XR_TYPE_EXTENSION_PROPERTIES });
	xrEnumerateInstanceExtensionProperties(nullptr, nInstanceExtensionProps, &nInstanceExtensionProps, extensionProps.data());
	res =  CreateXRInstance();
	return res;
}

XrResult XRAPI::CreateXRInstance()
{
	assert(m_instance == XR_NULL_HANDLE);
	std::vector<const char*> extensions;
	const std::vector<const char*> platformExtensions = platform->GetXRPlatformPlugin()->GetPlatformExtensions();
	std::transform(platformExtensions.begin(), platformExtensions.end(), std::back_inserter(extensions),
		[](const char* ext) { return ext; });
	const std::vector<const char*> graphicsExtensions = graphics->GetXRGraphicsPlugin()->GetGraphicsExtensions();
	std::transform(graphicsExtensions.begin(), graphicsExtensions.end(), std::back_inserter(extensions),
		[](const char* ext) { return ext; });


	XrInstanceCreateInfo createInfo = { XR_TYPE_INSTANCE_CREATE_INFO };
	createInfo.next = platform->GetXRPlatformPlugin()->GetInstanceCreateExtension();
	createInfo.createFlags = 0;
	createInfo.enabledApiLayerCount = 0;
	createInfo.enabledApiLayerNames = nullptr;
	createInfo.enabledExtensionCount = (uint32_t)extensions.size();
	createInfo.enabledExtensionNames = extensions.data();
	//std::string glExtension = "XR_KHR_opengl_enable";
	//const char* cGlStr = graphics->GetXRGraphicsPlugin()->GetGraphicsExtensionName();
	//createInfo.enabledExtensionNames = extensionNames.data();
	//createInfo.enabledExtensionNames = &cGlStr;
	strcpy_s(createInfo.applicationInfo.applicationName, "Portable Engine");
	createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;
	XrResult createInstanceResult = xrCreateInstance(&createInfo, &m_instance);
	if (!XR_SUCCEEDED(createInstanceResult))
	{
		throw "There is no XR instance";
	}
	return createInstanceResult;
}

void XRAPI::InitializeXRSystem()
{
	XrSystemGetInfo systemInfo{ XR_TYPE_SYSTEM_GET_INFO };
	systemInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	m_environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
	XrResult res = xrGetSystem(m_instance, &systemInfo, &m_systemId);
	graphics->GetXRGraphicsPlugin()->InitializeDeviceForXR(m_instance, m_systemId);
}

void XRAPI::InitializeXRSession()
{
	XrSessionCreateInfo createInfo{ XR_TYPE_SESSION_CREATE_INFO };
	createInfo.next = graphics->GetXRGraphicsPlugin()->GetGraphicsBinding();
	createInfo.systemId = m_systemId;
	XrResult res = xrCreateSession(m_instance, &createInfo, &m_session);


	XrReferenceSpaceCreateInfo referenceSpaceCreateInfo{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
	referenceSpaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
	XrPosef t{};
	t.orientation.w = 1;
	referenceSpaceCreateInfo.poseInReferenceSpace = t;
	res = xrCreateReferenceSpace(m_session, &referenceSpaceCreateInfo, &m_appSpace);


}

void XRAPI::CreateVisualizedSpaces()
{

}

void XRAPI::CreateSwapchains()
{
	XrResult res;
	XrSystemProperties systemProperties{ XR_TYPE_SYSTEM_PROPERTIES };
	res = xrGetSystemProperties(m_instance, m_systemId, &systemProperties);
	uint32_t viewCount;
	res = xrEnumerateViewConfigurationViews(m_instance, m_systemId, m_viewConfigType, 0, &viewCount, nullptr);
	m_configViews.resize(viewCount, { XR_TYPE_VIEW_CONFIGURATION_VIEW });
	res = xrEnumerateViewConfigurationViews(m_instance, m_systemId, m_viewConfigType, viewCount, &viewCount,
		m_configViews.data());

	// Create and cache view buffer for xrLocateViews later.
	m_views.resize(viewCount, { XR_TYPE_VIEW });

	// Create the swapchain and get the images.
	if (viewCount > 0) {
		// Select a swapchain format.
		uint32_t swapchainFormatCount;
		res = xrEnumerateSwapchainFormats(m_session, 0, &swapchainFormatCount, nullptr);
		std::vector<int64_t> swapchainFormats(swapchainFormatCount);
		res = xrEnumerateSwapchainFormats(m_session, (uint32_t)swapchainFormats.size(), &swapchainFormatCount,
			swapchainFormats.data());
		swapchainFormatCount == swapchainFormats.size();
		m_colorSwapchainFormat = graphics->GetXRGraphicsPlugin()->SelectColorSwapchainFormat(swapchainFormats);

		// Print swapchain formats and the selected one.
		//{
		//	std::string swapchainFormatsString;
		//	for (int64_t format : swapchainFormats) {
		//		const bool selected = format == m_colorSwapchainFormat;
		//		swapchainFormatsString += " ";
		//		if (selected) {
		//			swapchainFormatsString += "[";
		//		}
		//		swapchainFormatsString += std::to_string(format);
		//		if (selected) {
		//			swapchainFormatsString += "]";
		//		}
		//	}
			//Log::Write(Log::Level::Verbose, Fmt("Swapchain Formats: %s", swapchainFormatsString.c_str()));
		}

		// Create a swapchain for each view.
		for (uint32_t i = 0; i < viewCount; i++) {
			const XrViewConfigurationView& vp = m_configViews[i];
			//Log::Write(Log::Level::Info,
			//	Fmt("Creating swapchain for view %d with dimensions Width=%d Height=%d SampleCount=%d", i,
			//		vp.recommendedImageRectWidth, vp.recommendedImageRectHeight, vp.recommendedSwapchainSampleCount));

			// Create the swapchain.
			XrSwapchainCreateInfo swapchainCreateInfo{ XR_TYPE_SWAPCHAIN_CREATE_INFO };
			swapchainCreateInfo.arraySize = 1;
			swapchainCreateInfo.format = m_colorSwapchainFormat;
			swapchainCreateInfo.width = vp.recommendedImageRectWidth;
			swapchainCreateInfo.height = vp.recommendedImageRectHeight;
			swapchainCreateInfo.mipCount = 1;
			swapchainCreateInfo.faceCount = 1;
			// TODO: This method just returns a field in vp so I will do that directly for now. It might be a good idea to implement the method later though.
			//swapchainCreateInfo.sampleCount = m_graphicsPlugin->GetSupportedSwapchainSampleCount(vp);
			swapchainCreateInfo.sampleCount = vp.recommendedSwapchainSampleCount;
			swapchainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
			Swapchain swapchain;
			swapchain.width = swapchainCreateInfo.width;
			swapchain.height = swapchainCreateInfo.height;
			res = xrCreateSwapchain(m_session, &swapchainCreateInfo, &swapchain.handle);

			m_swapchains.push_back(swapchain);

			uint32_t imageCount;
			res = xrEnumerateSwapchainImages(swapchain.handle, 0, &imageCount, nullptr);
			// XXX This should really just return XrSwapchainImageBaseHeader*
			std::vector<XrSwapchainImageBaseHeader*> swapchainImages =
				graphics->GetXRGraphicsPlugin()->AllocateSwapchainImageStructs(imageCount, swapchainCreateInfo);
			res = xrEnumerateSwapchainImages(swapchain.handle, imageCount, &imageCount, swapchainImages[0]);

			m_swapchainImages.insert(std::make_pair(swapchain.handle, std::move(swapchainImages)));
		}
}

const XrEventDataBaseHeader* XRAPI::TryReadNextEvent() {
	// It is sufficient to clear the just the XrEventDataBuffer header to
	// XR_TYPE_EVENT_DATA_BUFFER
	XrEventDataBaseHeader* baseHeader = reinterpret_cast<XrEventDataBaseHeader*>(&m_eventDataBuffer);
	*baseHeader = { XR_TYPE_EVENT_DATA_BUFFER };
	const XrResult xr = xrPollEvent(m_instance, &m_eventDataBuffer);
	if (xr == XR_SUCCESS) {
		return baseHeader;
	}
	if (xr == XR_EVENT_UNAVAILABLE) {
		return nullptr;
	}
	throw "Bad poll event";
}

void XRAPI::PollEvents(/*bool* exitRenderLoop, bool* requestRestart*/) {
	//*exitRenderLoop = *requestRestart = false;

	// Process all pending messages.
	while (const XrEventDataBaseHeader* event = TryReadNextEvent()) {
		switch (event->type) {
		case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
			const auto& instanceLossPending = *reinterpret_cast<const XrEventDataInstanceLossPending*>(event);
			//*exitRenderLoop = true;
			//*requestRestart = true;
			return;
		}
		case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
			auto sessionStateChangedEvent = *reinterpret_cast<const XrEventDataSessionStateChanged*>(event);
			HandleSessionStateChangedEvent(sessionStateChangedEvent/*, exitRenderLoop, requestRestart*/);
			break;
		}
		//case XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING:
		//default: {
		//	Log::Write(Log::Level::Verbose, Fmt("Ignoring event type %d", event->type));
		//	break;
		//}
		}
	}
}

bool XRAPI::IsSessionRunning()
{
	return m_sessionRunning;
}

void XRAPI::RenderFrame()
{
	assert(m_session != XR_NULL_HANDLE);

	XrFrameWaitInfo frameWaitInfo{ XR_TYPE_FRAME_WAIT_INFO };
	XrFrameState frameState{ XR_TYPE_FRAME_STATE };
	assert(XR_SUCCEEDED(xrWaitFrame(m_session, &frameWaitInfo, &frameState)));

	XrFrameBeginInfo frameBeginInfo{ XR_TYPE_FRAME_BEGIN_INFO };
	assert(XR_SUCCEEDED(xrBeginFrame(m_session, &frameBeginInfo)));

	std::vector<XrCompositionLayerBaseHeader*> layers;
	XrCompositionLayerProjection layer{ XR_TYPE_COMPOSITION_LAYER_PROJECTION };
	std::vector<XrCompositionLayerProjectionView> projectionLayerViews;
	if (frameState.shouldRender == XR_TRUE) {
		if (RenderLayer(frameState.predictedDisplayTime, projectionLayerViews, layer)) {
			layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader*>(&layer));
		}
	}

	XrFrameEndInfo frameEndInfo{ XR_TYPE_FRAME_END_INFO };
	frameEndInfo.displayTime = frameState.predictedDisplayTime;
	frameEndInfo.environmentBlendMode = m_environmentBlendMode;
	frameEndInfo.layerCount = (uint32_t)layers.size();
	frameEndInfo.layers = layers.data();
	assert(XR_SUCCEEDED(xrEndFrame(m_session, &frameEndInfo)));

}

void XRAPI::HandleSessionStateChangedEvent(const XrEventDataSessionStateChanged& stateChangedEvent/*, bool* exitRenderLoop,
	bool* requestRestart*/) {
	const XrSessionState oldState = m_sessionState;
	m_sessionState = stateChangedEvent.state;

	//Log::Write(Log::Level::Info, Fmt("XrEventDataSessionStateChanged: state %s->%s session=%lld time=%lld", to_string(oldState),
	//	to_string(m_sessionState), stateChangedEvent.session, stateChangedEvent.time));

	//if ((stateChangedEvent.session != XR_NULL_HANDLE) && (stateChangedEvent.session != m_session)) {
	//	Log::Write(Log::Level::Error, "XrEventDataSessionStateChanged for unknown session");
	//	return;
	//}

	switch (m_sessionState) {
	case XR_SESSION_STATE_READY: {
		assert(m_session != XR_NULL_HANDLE);
		XrSessionBeginInfo sessionBeginInfo{ XR_TYPE_SESSION_BEGIN_INFO };
		sessionBeginInfo.primaryViewConfigurationType = m_viewConfigType;
		assert(XR_SUCCEEDED(xrBeginSession(m_session, &sessionBeginInfo)));
		m_sessionRunning = true;
		break;
	}
	case XR_SESSION_STATE_STOPPING: {
		assert(m_session != XR_NULL_HANDLE);
		m_sessionRunning = false;
		assert(XR_SUCCEEDED(xrEndSession(m_session)));
			break;
	}
	case XR_SESSION_STATE_EXITING: {
		//*exitRenderLoop = true;
		// Do not attempt to restart because user closed this session.
		//*requestRestart = false;
		break;
	}
	case XR_SESSION_STATE_LOSS_PENDING: {
		//*exitRenderLoop = true;
		// Poll for a new instance.
		//*requestRestart = true;
		break;
	}
	default:
		break;
	}
}

bool XRAPI::RenderLayer(XrTime predictedDisplayTime, std::vector<XrCompositionLayerProjectionView>& projectionLayerViews,
	XrCompositionLayerProjection& layer) {
	XrResult res;

	XrViewState viewState{ XR_TYPE_VIEW_STATE };
	uint32_t viewCapacityInput = (uint32_t)m_views.size();
	uint32_t viewCountOutput;

	XrViewLocateInfo viewLocateInfo{ XR_TYPE_VIEW_LOCATE_INFO };
	viewLocateInfo.viewConfigurationType = m_viewConfigType;
	viewLocateInfo.displayTime = predictedDisplayTime;
	viewLocateInfo.space = m_appSpace;

	res = xrLocateViews(m_session, &viewLocateInfo, &viewState, viewCapacityInput, &viewCountOutput, m_views.data());
	assert(res == XR_SUCCESS);
	if ((viewState.viewStateFlags & XR_VIEW_STATE_POSITION_VALID_BIT) == 0 ||
		(viewState.viewStateFlags & XR_VIEW_STATE_ORIENTATION_VALID_BIT) == 0) {
		return false;  // There is no valid tracking poses for the views.
	}

	assert(viewCountOutput == viewCapacityInput);
	assert(viewCountOutput == m_configViews.size());
	assert(viewCountOutput == m_swapchains.size());

	projectionLayerViews.resize(viewCountOutput);

	// For each locatable space that we want to visualize, render a 25cm cube.
	//std::vector<Cube> cubes;

	//for (XrSpace visualizedSpace : m_visualizedSpaces) {
	//	XrSpaceLocation spaceLocation{ XR_TYPE_SPACE_LOCATION };
	//	res = xrLocateSpace(visualizedSpace, m_appSpace, predictedDisplayTime, &spaceLocation);
	//	assert(res == XR_SUCCESS);
	//	if (XR_UNQUALIFIED_SUCCESS(res)) {
	//		if ((spaceLocation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
	//			(spaceLocation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
	//			cubes.push_back(Cube{ spaceLocation.pose, {0.25f, 0.25f, 0.25f} });
	//		}
	//	}
	//	else {
	//		Log::Write(Log::Level::Verbose, Fmt("Unable to locate a visualized reference space in app space: %d", res));
	//	}
	//}

	// Render a 10cm cube scaled by grabAction for each hand. Note renderHand will only be
	// true when the application has focus.
	//for (auto hand : { Side::LEFT, Side::RIGHT }) {
	//	XrSpaceLocation spaceLocation{ XR_TYPE_SPACE_LOCATION };
	//	res = xrLocateSpace(m_input.handSpace[hand], m_appSpace, predictedDisplayTime, &spaceLocation);
	//	CHECK_XRRESULT(res, "xrLocateSpace");
	//	if (XR_UNQUALIFIED_SUCCESS(res)) {
	//		if ((spaceLocation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
	//			(spaceLocation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
	//			float scale = 0.1f * m_input.handScale[hand];
	//			cubes.push_back(Cube{ spaceLocation.pose, {scale, scale, scale} });
	//		}
	//	}
	//	else {
	//		// Tracking loss is expected when the hand is not active so only log a message
	//		// if the hand is active.
	//		if (m_input.handActive[hand] == XR_TRUE) {
	//			const char* handName[] = { "left", "right" };
	//			Log::Write(Log::Level::Verbose,
	//				Fmt("Unable to locate %s hand action space in app space: %d", handName[hand], res));
	//		}
	//	}
	//}

	// Render view to the appropriate part of the swapchain image.
	for (uint32_t i = 0; i < viewCountOutput; i++) {
		// Each view has a separate swapchain which is acquired, rendered to, and released.
		const Swapchain viewSwapchain = m_swapchains[i];

		XrSwapchainImageAcquireInfo acquireInfo{ XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };

		uint32_t swapchainImageIndex;
		assert(XR_SUCCEEDED(xrAcquireSwapchainImage(viewSwapchain.handle, &acquireInfo, &swapchainImageIndex)));

		XrSwapchainImageWaitInfo waitInfo{ XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO };
		waitInfo.timeout = XR_INFINITE_DURATION;
		assert(XR_SUCCEEDED(xrWaitSwapchainImage(viewSwapchain.handle, &waitInfo)));

		projectionLayerViews[i] = { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW };
		projectionLayerViews[i].pose = m_views[i].pose;
		projectionLayerViews[i].fov = m_views[i].fov;
		projectionLayerViews[i].subImage.swapchain = viewSwapchain.handle;
		projectionLayerViews[i].subImage.imageRect.offset = { 0, 0 };
		projectionLayerViews[i].subImage.imageRect.extent = { viewSwapchain.width, viewSwapchain.height };

		const XrSwapchainImageBaseHeader* const swapchainImage = m_swapchainImages[viewSwapchain.handle][swapchainImageIndex];
		graphics->GetXRGraphicsPlugin()->RenderView(projectionLayerViews[i], swapchainImage, m_colorSwapchainFormat /*cubes*/);

		XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
		assert(XR_SUCCEEDED(xrReleaseSwapchainImage(viewSwapchain.handle, &releaseInfo)));
	}

	layer.space = m_appSpace;
	layer.viewCount = (uint32_t)projectionLayerViews.size();
	layer.views = projectionLayerViews.data();
	return true;
}