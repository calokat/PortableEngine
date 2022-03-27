#ifndef __EMSCRIPTEN__
#include "XRAPI.h"
#include <iostream>
#include <assert.h>
#include "TransformSystem.h"
#ifdef _WIN64
#include "../PE_XR/Graphics_Plugins/DirectX11/XRGraphicsPlugin_DirectX11.h"
#include "../PE_XR/Platform_Plugins/Win32/XRPlatformPlugin_Win32.h"
#endif
#include "openxr/openxr_platform.h"
#include "../PE_XR/Graphics_Plugins/OpenGL/XRGraphicsPlugin_OpenGL.h"
XRAPI::XRAPI(IPlatform* plat, IGraphicsAPI* graph, GameWindow* window, Options options)
{
	switch (options.graphicsAPI)
	{
	case PE::GraphicsAPI::OpenGL:
		graphicsAPI = GraphicsAPI::GRAPHICS_OPENGL;
		graphicsPlugin = new XRGraphicsPlugin_OpenGL((OpenGLAPI*)graph, window);
		break;
	case PE::GraphicsAPI::DirectX11:
		graphicsAPI = GraphicsAPI::GRAPHICS_D3D;
		graphicsPlugin = new XRGraphicsPlugin_DirectX11((DirectXAPI*)graph);
		break;
	default:
		graphicsAPI = GraphicsAPI::GRAPHICS_OPENGL_ES;
	}
	switch (options.platform)
	{
	case PE::Platform::Win32:
		platformPlugin = new XRPlatformPlugin_Win32();
	}
	Init();
	InitializeXRSystem();
	InitializeXRSession();
	InitializeActions();
	CreateSwapchains(window);
}

XRAPI::~XRAPI()
{
	delete platformPlugin;
	delete graphicsPlugin;
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
	const std::vector<const char*> platformExtensions = platformPlugin->GetPlatformExtensions();
	std::transform(platformExtensions.begin(), platformExtensions.end(), std::back_inserter(extensions),
		[](const char* ext) { return ext; });
	const std::vector<const char*> graphicsExtensions = graphicsPlugin->GetGraphicsExtensions();
	std::transform(graphicsExtensions.begin(), graphicsExtensions.end(), std::back_inserter(extensions),
		[](const char* ext) { return ext; });


	XrInstanceCreateInfo createInfo = { XR_TYPE_INSTANCE_CREATE_INFO };
	createInfo.next = platformPlugin->GetInstanceCreateExtension();
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
	graphicsPlugin->InitializeDeviceForXR(m_instance, m_systemId);
}

void XRAPI::InitializeXRSession()
{
	XrSessionCreateInfo createInfo{ XR_TYPE_SESSION_CREATE_INFO };
	createInfo.next = graphicsPlugin->GetGraphicsBinding();
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

void XRAPI::CreateSwapchains(GameWindow* window)
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
		m_colorSwapchainFormat = graphicsPlugin->SelectColorSwapchainFormat(swapchainFormats);

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
				graphicsPlugin->AllocateSwapchainImageStructs(imageCount, swapchainCreateInfo);
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

void XRAPI::UpdateDevices(XrTime predictedTime, entt::basic_view<entt::entity, entt::get_t<XRDevice, Transform>, entt::exclude_t<>, void> xrDevices, entt::basic_view<entt::entity, entt::get_t<Camera, Transform>, entt::exclude_t<>, void> cameraTransformView)
{
	auto [cam, camTransform] = cameraTransformView.get(cameraTransformView.front());
	if (!viewCams.empty()) cam = viewCams[0];
	for (auto devIt = xrDevices.begin(); devIt != xrDevices.end(); ++devIt)
	{
		auto [dev, t] = xrDevices.get(*devIt);
		switch (dev.type)
		{
		case XRDeviceType::LeftHand:
		case XRDeviceType::RightHand:
			XrSpaceLocation spaceLocation{ XR_TYPE_SPACE_LOCATION };
			XrResult res = xrLocateSpace(inputState.handSpace[dev.type], m_appSpace, predictedTime, &spaceLocation);
			if (XR_UNQUALIFIED_SUCCESS(res)) {
				if ((spaceLocation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
					(spaceLocation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
					XrVector3f posePos = spaceLocation.pose.position;
					t.position = glm::vec3(posePos.x, posePos.y, posePos.z) + camTransform.position;
					XrQuaternionf poseQuat = spaceLocation.pose.orientation;
					t.orientation = glm::quat(poseQuat.w, poseQuat.x, poseQuat.y, poseQuat.z);
					TransformSystem::CalculateWorldMatrix(&t);
				}
			}
			else {
			}

		}

	}
}

void XRAPI::Frame(entt::registry& reg, IRenderSystem* renderSystem)
{
	XrFrameState frameState = BeginFrame();
	LocateViews(frameState.predictedDisplayTime);
	PollActions();
	UpdateDevices(frameState.predictedDisplayTime, reg.view<XRDevice, Transform>(), reg.view<Camera, Transform>());
	auto camView = reg.view<Camera, Transform>();
	auto camTransform = camView.get<Transform>(camView.front());
	CalculateCameraViews(camTransform);
	RenderFrame(reg, renderSystem, frameState);

}

void XRAPI::RenderFrame(entt::registry& reg, IRenderSystem* renderSystem, XrFrameState frameState)
{
	std::vector<XrCompositionLayerBaseHeader*> layers;
	XrCompositionLayerProjection layer{ XR_TYPE_COMPOSITION_LAYER_PROJECTION };
	std::vector<XrCompositionLayerProjectionView> projectionLayerViews;
	if (frameState.shouldRender == XR_TRUE) {
		if (RenderLayer(frameState.predictedDisplayTime, projectionLayerViews, layer, reg, renderSystem)) {
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

bool XRAPI::LocateViews(XrTime predictedDisplayTime)
{
	XrResult res;

	XrViewState viewState{ XR_TYPE_VIEW_STATE };
	uint32_t viewCapacityInput = (uint32_t)m_views.size();
	uint32_t viewCountOutput;

	XrViewLocateInfo viewLocateInfo{ XR_TYPE_VIEW_LOCATE_INFO };
	viewLocateInfo.viewConfigurationType = m_viewConfigType;
	viewLocateInfo.displayTime = predictedDisplayTime;
	viewLocateInfo.space = m_appSpace;

	res = xrLocateViews(m_session, &viewLocateInfo, &viewState, viewCapacityInput, &viewCountOutput, m_views.data());
	assert(XR_SUCCEEDED(res));
	assert(viewCountOutput == viewCapacityInput);
	assert(viewCountOutput == m_configViews.size());
	assert(viewCountOutput == m_swapchains.size());

	if ((viewState.viewStateFlags & XR_VIEW_STATE_POSITION_VALID_BIT) == 0 ||
		(viewState.viewStateFlags & XR_VIEW_STATE_ORIENTATION_VALID_BIT) == 0) {
		return false;  // There is no valid tracking poses for the views.
	}
	return true;
}

void XRAPI::CalculateCameraViews(const Transform& primaryCamTransform)
{
	viewCams.resize(m_views.size());
	for (int i = 0; i < m_views.size(); ++i)
	{
		const auto& pose = m_views[i].pose;
		XrMatrix4x4f proj;
		XrMatrix4x4f_CreateProjectionFov(&proj, graphicsAPI, m_views[i].fov, 0.05f, 100.0f);
		XrMatrix4x4f toView;
		XrVector3f scale{ 1.f, 1.f, 1.f };
		XrVector3f camOriginalPos = { primaryCamTransform.position.x, primaryCamTransform.position.y, primaryCamTransform.position.z };
		XrVector3f finalPos;
		XrVector3f_Add(&finalPos, &camOriginalPos, &pose.position);
		XrMatrix4x4f_CreateTranslationRotationScale(&toView, &finalPos, &pose.orientation, &scale);
		XrMatrix4x4f view;
		XrMatrix4x4f_InvertRigidBody(&view, &toView);

		Camera toAdd;
		toAdd.view = glm::make_mat4(view.m);
		toAdd.projection = glm::make_mat4(proj.m);
		
		viewCams[i] = toAdd;
	}
}

void XRAPI::InitializeActions()
{
	// Create an action set.
	{
		XrActionSetCreateInfo actionSetInfo{ XR_TYPE_ACTION_SET_CREATE_INFO };
		strcpy_s(actionSetInfo.actionSetName, "gameplay");
		strcpy_s(actionSetInfo.localizedActionSetName, "Gameplay");
		actionSetInfo.priority = 0;
		xrCreateActionSet(m_instance, &actionSetInfo, &inputState.actionSet);
	}

	// Get the XrPath for the left and right hands - we will use them as subaction paths.
	xrStringToPath(m_instance, "/user/hand/left", &inputState.handSubactionPath[Side::LEFT]);
	xrStringToPath(m_instance, "/user/hand/right", &inputState.handSubactionPath[Side::RIGHT]);

	{
		XrActionCreateInfo actionInfo{ XR_TYPE_ACTION_CREATE_INFO };
		// Create an input action getting the left and right hand poses.
		actionInfo.actionType = XR_ACTION_TYPE_POSE_INPUT;
		strcpy_s(actionInfo.actionName, "hand_pose");
		strcpy_s(actionInfo.localizedActionName, "Hand Pose");
		actionInfo.countSubactionPaths = uint32_t(inputState.handSubactionPath.size());
		actionInfo.subactionPaths = inputState.handSubactionPath.data();
		xrCreateAction(inputState.actionSet, &actionInfo, &inputState.poseAction);
	}

	std::array<XrPath, 2> posePath;
	xrStringToPath(m_instance, "/user/hand/left/input/grip/pose", &posePath[Side::LEFT]);
	xrStringToPath(m_instance, "/user/hand/right/input/grip/pose", &posePath[Side::RIGHT]);

	{
		XrPath oculusTouchInteractionProfilePath;
		xrStringToPath(m_instance, "/interaction_profiles/oculus/touch_controller", &oculusTouchInteractionProfilePath);
		std::vector<XrActionSuggestedBinding> bindings{{
														{inputState.poseAction, posePath[Side::LEFT]},
														{inputState.poseAction, posePath[Side::RIGHT]}
		}};
		XrInteractionProfileSuggestedBinding suggestedBindings{ XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING };
		suggestedBindings.interactionProfile = oculusTouchInteractionProfilePath;
		suggestedBindings.suggestedBindings = bindings.data();
		suggestedBindings.countSuggestedBindings = (uint32_t)bindings.size();
		xrSuggestInteractionProfileBindings(m_instance, &suggestedBindings);
	}

	// Suggest bindings for the Valve Index Controller.
	{
		XrPath indexControllerInteractionProfilePath;
		xrStringToPath(m_instance, "/interaction_profiles/valve/index_controller", &indexControllerInteractionProfilePath);
		//std::vector<XrActionSuggestedBinding> bindings{ {{m_input.grabAction, squeezeForcePath[Side::LEFT]},
		//												{m_input.grabAction, squeezeForcePath[Side::RIGHT]},
		//												{m_input.poseAction, posePath[Side::LEFT]},
		//												{m_input.poseAction, posePath[Side::RIGHT]},
		//												{m_input.quitAction, bClickPath[Side::LEFT]},
		//												{m_input.quitAction, bClickPath[Side::RIGHT]},
		//												{m_input.vibrateAction, hapticPath[Side::LEFT]},
		//												{m_input.vibrateAction, hapticPath[Side::RIGHT]}} };
		std::vector<XrActionSuggestedBinding> bindings{
			{inputState.poseAction, posePath[Side::LEFT]},
			{inputState.poseAction, posePath[Side::RIGHT]}
		};
		XrInteractionProfileSuggestedBinding suggestedBindings{ XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING };
		suggestedBindings.interactionProfile = indexControllerInteractionProfilePath;
		suggestedBindings.suggestedBindings = bindings.data();
		suggestedBindings.countSuggestedBindings = (uint32_t)bindings.size();
		xrSuggestInteractionProfileBindings(m_instance, &suggestedBindings);
	}

	XrActionSpaceCreateInfo actionSpaceInfo{ XR_TYPE_ACTION_SPACE_CREATE_INFO };
	actionSpaceInfo.action = inputState.poseAction;
	actionSpaceInfo.poseInActionSpace.orientation.w = 1.f;
	actionSpaceInfo.subactionPath = inputState.handSubactionPath[Side::LEFT];
	xrCreateActionSpace(m_session, &actionSpaceInfo, &inputState.handSpace[Side::LEFT]);
	actionSpaceInfo.subactionPath = inputState.handSubactionPath[Side::RIGHT];
	xrCreateActionSpace(m_session, &actionSpaceInfo, &inputState.handSpace[Side::RIGHT]);

	XrSessionActionSetsAttachInfo attachInfo{ XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO };
	attachInfo.countActionSets = 1;
	attachInfo.actionSets = &inputState.actionSet;
	xrAttachSessionActionSets(m_session, &attachInfo);
}

void XRAPI::PollActions()
{
	// Sync actions
	const XrActiveActionSet activeActionSet{ inputState.actionSet, XR_NULL_PATH };
	XrActionsSyncInfo syncInfo{ XR_TYPE_ACTIONS_SYNC_INFO };
	syncInfo.countActiveActionSets = 1;
	syncInfo.activeActionSets = &activeActionSet;
	XrResult res = xrSyncActions(m_session, &syncInfo);
}

XrFrameState XRAPI::BeginFrame()
{
	assert(m_session != XR_NULL_HANDLE);

	XrFrameWaitInfo frameWaitInfo{ XR_TYPE_FRAME_WAIT_INFO };
	XrFrameState frameState{ XR_TYPE_FRAME_STATE };
	assert(XR_SUCCEEDED(xrWaitFrame(m_session, &frameWaitInfo, &frameState)));

	XrFrameBeginInfo frameBeginInfo{ XR_TYPE_FRAME_BEGIN_INFO };
	assert(XR_SUCCEEDED(xrBeginFrame(m_session, &frameBeginInfo)));

	return frameState;
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
	XrCompositionLayerProjection& layer, entt::registry& reg, IRenderSystem* renderSystem) {
	projectionLayerViews.resize(m_views.size());
	// Render view to the appropriate part of the swapchain image.
	for (uint32_t i = 0; i < m_views.size(); i++) {
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
		graphicsPlugin->RenderView(projectionLayerViews[i], swapchainImage, m_colorSwapchainFormat, reg, renderSystem, viewCams[i]);

		XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
		assert(XR_SUCCEEDED(xrReleaseSwapchainImage(viewSwapchain.handle, &releaseInfo)));
	}

	layer.space = m_appSpace;
	layer.viewCount = (uint32_t)projectionLayerViews.size();
	layer.views = projectionLayerViews.data();
	return true;
}
#endif