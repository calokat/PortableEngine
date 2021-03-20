#include "XRAPI.h"
#include <iostream>
XRAPI::XRAPI(IPlatform* plat, IGraphicsAPI* graph) : platform(plat), graphics(graph)
{
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


}

XrResult XRAPI::CreateXRInstance()
{
	XrInstanceCreateInfo createInfo = { XR_TYPE_INSTANCE_CREATE_INFO };
	createInfo.createFlags = 0;
	createInfo.enabledApiLayerCount = 0;
	createInfo.enabledApiLayerNames = nullptr;
	createInfo.enabledExtensionCount = 1;
	std::string glExtension = "XR_KHR_opengl_enable";
	const char* cGlStr = glExtension.c_str();
	//createInfo.enabledExtensionNames = extensionNames.data();
	createInfo.enabledExtensionNames = &cGlStr;
	strcpy_s(createInfo.applicationInfo.applicationName, "HelloOpenXR");
	createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;
	XrResult createInstanceResult = xrCreateInstance(&createInfo, &m_instance);
}
