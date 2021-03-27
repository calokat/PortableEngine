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
