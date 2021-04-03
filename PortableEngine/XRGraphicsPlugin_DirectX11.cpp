#include "XRGraphicsPlugin_DirectX11.h"
#include <openxr/xr_linear.h>
#include "DIrectXRenderer.h"
#pragma comment(lib, "dxgi.lib")

XRGraphicsPlugin_DirectX11::XRGraphicsPlugin_DirectX11(Microsoft::WRL::ComPtr<ID3D11DeviceContext> ctx, Microsoft::WRL::ComPtr<ID3D11Device> dev, GameWindow* win)
    : m_deviceContext(ctx), m_device(dev), window(win)
{
}

std::vector<const char*> XRGraphicsPlugin_DirectX11::GetGraphicsExtensions()
{
    return { "XR_KHR_D3D11_enable" };
}

void XRGraphicsPlugin_DirectX11::InitializeDeviceForXR(XrInstance instance, XrSystemId systemId)
{
    PFN_xrGetD3D11GraphicsRequirementsKHR pfnGetD3D11GraphicsRequirementsKHR = nullptr;
    assert(XR_SUCCEEDED(xrGetInstanceProcAddr(instance, "xrGetD3D11GraphicsRequirementsKHR",
        reinterpret_cast<PFN_xrVoidFunction*>(&pfnGetD3D11GraphicsRequirementsKHR))));

    // Create the D3D11 device for the adapter associated with the system.
    XrGraphicsRequirementsD3D11KHR graphicsRequirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_D3D11_KHR };
    assert(XR_SUCCEEDED(pfnGetD3D11GraphicsRequirementsKHR(instance, systemId, &graphicsRequirements)));
    const Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter = GetAdapter(graphicsRequirements.adapterLuid);

    std::vector<D3D_FEATURE_LEVEL> featureLevels = { D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0, D3D_FEATURE_LEVEL_11_1,
                                                D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
    featureLevels.erase(std::remove_if(featureLevels.begin(), featureLevels.end(),
        [&](D3D_FEATURE_LEVEL fl) { return fl < graphicsRequirements.minFeatureLevel; }),
        featureLevels.end());
    assert(featureLevels.size() != 0);

    //InitializeD3D11DeviceForAdapter(adapter.Get(), featureLevels, m_device.ReleaseAndGetAddressOf(),
    //    m_deviceContext.ReleaseAndGetAddressOf());

    //InitializeResources();

    m_graphicsBinding.device = m_device.Get();
}

const XrBaseInStructure* XRGraphicsPlugin_DirectX11::GetGraphicsBinding()
{
    return reinterpret_cast<const XrBaseInStructure*>(&m_graphicsBinding);
}

int64_t XRGraphicsPlugin_DirectX11::SelectColorSwapchainFormat(const std::vector<int64_t>& runtimeFormats) const
{
    // List of supported color swapchain formats.
    constexpr DXGI_FORMAT SupportedColorSwapchainFormats[] = {
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_B8G8R8A8_UNORM,
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
    };

    auto swapchainFormatIt =
        std::find_first_of(runtimeFormats.begin(), runtimeFormats.end(), std::begin(SupportedColorSwapchainFormats),
            std::end(SupportedColorSwapchainFormats));
    if (swapchainFormatIt == runtimeFormats.end()) {
        throw ("No runtime swapchain format supported for color swapchain");
    }

    return *swapchainFormatIt;
}

std::vector<XrSwapchainImageBaseHeader*> XRGraphicsPlugin_DirectX11::AllocateSwapchainImageStructs(uint32_t capacity, const XrSwapchainCreateInfo&)
{
    // Allocate and initialize the buffer of image structs (must be sequential in memory for xrEnumerateSwapchainImages).
    // Return back an array of pointers to each swapchain image struct so the consumer doesn't need to know the type/size.
    std::vector<XrSwapchainImageD3D11KHR> swapchainImageBuffer(capacity);
    std::vector<XrSwapchainImageBaseHeader*> swapchainImageBase;
    for (XrSwapchainImageD3D11KHR& image : swapchainImageBuffer) {
        image.type = XR_TYPE_SWAPCHAIN_IMAGE_D3D11_KHR;
        swapchainImageBase.push_back(reinterpret_cast<XrSwapchainImageBaseHeader*>(&image));
    }

    // Keep the buffer alive by moving it into the list of buffers.
    m_swapchainImageBuffers.push_back(std::move(swapchainImageBuffer));

    return swapchainImageBase;
}

void XRGraphicsPlugin_DirectX11::RenderView(const XrCompositionLayerProjectionView& layerView, const XrSwapchainImageBaseHeader* swapchainImage, int64_t swapchainFormat, entt::registry& reg, IRenderSystem* renderSystem)
{
    assert(layerView.subImage.imageArrayIndex == 0);  // Texture arrays not supported.

    ID3D11Texture2D* const colorTexture = reinterpret_cast<const XrSwapchainImageD3D11KHR*>(swapchainImage)->texture;

    CD3D11_VIEWPORT viewport((float)layerView.subImage.imageRect.offset.x, (float)layerView.subImage.imageRect.offset.y,
        (float)layerView.subImage.imageRect.extent.width,
        (float)layerView.subImage.imageRect.extent.height);
    m_deviceContext->RSSetViewports(1, &viewport);

    // Create RenderTargetView with original swapchain format (swapchain is typeless).
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
    const CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2D, (DXGI_FORMAT)swapchainFormat);
    m_device->CreateRenderTargetView(colorTexture, &renderTargetViewDesc, renderTargetView.ReleaseAndGetAddressOf());

    const Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = GetDepthStencilView(colorTexture);

    // Clear swapchain and depth buffer. NOTE: This will clear the entire render target view, not just the specified view.
    // TODO: Do not clear to a color when using a pass-through view configuration.
    const float color[] = { 0.4f, 0.6f, 0.75f, 0.0f };
    m_deviceContext->ClearRenderTargetView(renderTargetView.Get(),  color);
    m_deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    ID3D11RenderTargetView* renderTargets[] = { renderTargetView.Get() };
    m_deviceContext->OMSetRenderTargets((UINT)std::size(renderTargets), renderTargets, depthStencilView.Get());

    const auto& pose = layerView.pose;
    XrMatrix4x4f proj;
    XrMatrix4x4f_CreateProjectionFov(&proj, GRAPHICS_D3D, layerView.fov, 0.05f, 100.0f);
    XrMatrix4x4f toView;
    XrVector3f scale{ 1.f, 1.f, 1.f };
    XrMatrix4x4f_CreateTranslationRotationScale(&toView, &pose.position, &pose.orientation, &scale);
    XrMatrix4x4f view;
    XrMatrix4x4f_InvertRigidBody(&view, &toView);
    XrMatrix4x4f vp;
    XrMatrix4x4f_Multiply(&vp, &proj, &view);

    auto camEntityView = reg.view<Camera>();
    auto [camera, camTransform] = reg.get<Camera, Transform>(camEntityView[0]);

    camera.view = glm::make_mat4(view.m);
    camera.projection = glm::make_mat4(proj.m);

    auto renderableView = reg.view<DirectXRenderer, Transform>();
    for (auto renderable : renderableView)
    {
        DirectXRenderer& renderer = reg.get<DirectXRenderer>(renderable);
        Mesh& mesh = reg.get<Mesh>(renderable);
        Transform& meshTransform = reg.get<Transform>(renderable);
        //renderSystem->LoadMesh(&renderer, mesh);
        renderSystem->BindRenderer(&renderer);

        renderSystem->UpdateRenderer(&renderer, meshTransform, camera);
        renderSystem->Draw(&renderer);
    }


    //const XMMATRIX spaceToView = XMMatrixInverse(nullptr, LoadXrPose(layerView.pose));
    //XrMatrix4x4f projectionMatrix;
    //XrMatrix4x4f_CreateProjectionFov(&projectionMatrix, GRAPHICS_D3D, layerView.fov, 0.05f, 100.0f);

    // Set shaders and constant buffers.
    //ViewProjectionConstantBuffer viewProjection;
    //XMStoreFloat4x4(&viewProjection.ViewProjection, XMMatrixTranspose(spaceToView * LoadXrMatrix(projectionMatrix)));
    //m_deviceContext->UpdateSubresource(m_viewProjectionCBuffer.Get(), 0, nullptr, &viewProjection, 0, 0);

    //ID3D11Buffer* const constantBuffers[] = { m_modelCBuffer.Get(), m_viewProjectionCBuffer.Get() };
    //m_deviceContext->VSSetConstantBuffers(0, (UINT)ArraySize(constantBuffers), constantBuffers);
    //m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    //m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // Set cube primitive data.
    //const UINT strides[] = { sizeof(Geometry::Vertex) };
    //const UINT offsets[] = { 0 };
    //ID3D11Buffer* vertexBuffers[] = { m_cubeVertexBuffer.Get() };
    //m_deviceContext->IASetVertexBuffers(0, (UINT)ArraySize(vertexBuffers), vertexBuffers, strides, offsets);
    //m_deviceContext->IASetIndexBuffer(m_cubeIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
    //m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //m_deviceContext->IASetInputLayout(m_inputLayout.Get());

    // Render each cube
    //for (const Cube& cube : cubes) {
    //    // Compute and update the model transform.
    //    ModelConstantBuffer model;
    //    XMStoreFloat4x4(&model.Model,
    //        XMMatrixTranspose(XMMatrixScaling(cube.Scale.x, cube.Scale.y, cube.Scale.z) * LoadXrPose(cube.Pose)));
    //    m_deviceContext->UpdateSubresource(m_modelCBuffer.Get(), 0, nullptr, &model, 0, 0);

    //    // Draw the cube.
    //    m_deviceContext->DrawIndexed((UINT)ArraySize(Geometry::c_cubeIndices), 0, 0);
    //}
}

Microsoft::WRL::ComPtr<IDXGIAdapter1> XRGraphicsPlugin_DirectX11::GetAdapter(LUID adapterId)
{
    // Create the DXGI factory.
    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
    assert(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory.ReleaseAndGetAddressOf())) == S_OK);

    for (UINT adapterIndex = 0;; adapterIndex++) {
        // EnumAdapters1 will fail with DXGI_ERROR_NOT_FOUND when there are no more adapters to enumerate.
        Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter;
        assert(dxgiFactory->EnumAdapters1(adapterIndex, dxgiAdapter.ReleaseAndGetAddressOf()) == S_OK);

        DXGI_ADAPTER_DESC1 adapterDesc;
        assert(dxgiAdapter->GetDesc1(&adapterDesc) == S_OK);
        if (memcmp(&adapterDesc.AdapterLuid, &adapterId, sizeof(adapterId)) == 0) {
            //Log::Write(Log::Level::Verbose, Fmt("Using graphics adapter %ws", adapterDesc.Description));
            return dxgiAdapter;
        }
    }
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> XRGraphicsPlugin_DirectX11::GetDepthStencilView(ID3D11Texture2D* colorTexture)
{
    // If a depth-stencil view has already been created for this back-buffer, use it.
    auto depthBufferIt = m_colorToDepthMap.find(colorTexture);
    if (depthBufferIt != m_colorToDepthMap.end()) {
        return depthBufferIt->second;
    }

    // This back-buffer has no corresponding depth-stencil texture, so create one with matching dimensions.
    D3D11_TEXTURE2D_DESC colorDesc;
    colorTexture->GetDesc(&colorDesc);

    D3D11_TEXTURE2D_DESC depthDesc{};
    depthDesc.Width = colorDesc.Width;
    depthDesc.Height = colorDesc.Height;
    depthDesc.ArraySize = colorDesc.ArraySize;
    depthDesc.MipLevels = 1;
    depthDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    depthDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
    depthDesc.SampleDesc.Count = 1;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture;
    assert(m_device->CreateTexture2D(&depthDesc, nullptr, depthTexture.ReleaseAndGetAddressOf()) == S_OK);

    // Create and cache the depth stencil view.
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D32_FLOAT);
    assert(m_device->CreateDepthStencilView(depthTexture.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf()) == S_OK);
    depthBufferIt = m_colorToDepthMap.insert(std::make_pair(colorTexture, depthStencilView)).first;

    return depthStencilView;
}
