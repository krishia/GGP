#include "Renderer/Renderer.h"

namespace library
{
    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
      Method:   Renderer::Renderer

      Summary:  Constructor

       Modifies: [m_driverType, m_featureLevel, m_d3dDevice, m_d3dDevice1,
                  m_immediateContext, m_immediateContext1, m_swapChain,
                  m_swapChain1, m_renderTargetView, m_vertexShader,
                  m_pixelShader, m_vertexLayout, m_vertexBuffer].
    M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
    /*--------------------------------------------------------------------
      TODO: Renderer::Renderer definition (remove the comment)
    --------------------------------------------------------------------*/
    Renderer::Renderer()
        : m_driverType(D3D_DRIVER_TYPE_NULL)
        , m_featureLevel(D3D_FEATURE_LEVEL_11_0)
        , m_d3dDevice(nullptr)
        , m_d3dDevice1(nullptr)
        , m_immediateContext(nullptr)
        , m_immediateContext1(nullptr)
        , m_swapChain(nullptr)
        , m_swapChain1(nullptr)
        , m_renderTargetView(nullptr)
        , m_depthStencil(nullptr)
        , m_depthStencilView(nullptr)
        , m_camera(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f))
        , m_projection()
        , m_renderables()
        , m_vertexShaders()
        , m_pixelShaders()
    {

    }
    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
      Method:   Renderer::Initialize

      Summary:  Creates Direct3D device and swap chain

      Args:     HWND hWnd
                  Handle to the window

      Modifies: [m_d3dDevice, m_featureLevel, m_immediateContext,
                  m_d3dDevice1, m_immediateContext1, m_swapChain1,
                  m_swapChain, m_renderTargetView, m_vertexShader, 
                  m_vertexLayout, m_pixelShader, m_vertexBuffer].

      Returns:  HRESULT
                  Status code
    M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
    /*--------------------------------------------------------------------
      TODO: Renderer::Initialize definition (remove the comment)
    --------------------------------------------------------------------*/
    HRESULT Renderer::Initialize(_In_ HWND hWnd)
    {
        HRESULT hr = S_OK;

        RECT rc;

        GetClientRect(hWnd, &rc);
        UINT width = (UINT(rc.right - rc.left));
        UINT height = (UINT(rc.bottom - rc.top));
        UINT createDeviceFlags = 0;
        POINT p1, p2;
        p1.x = rc.left;
        p1.y = rc.top;
        p2.x = rc.right;
        p2.y = rc.bottom;

        ClientToScreen(hWnd, &p1);
        ClientToScreen(hWnd, &p2);

        rc.left = p1.x;
        rc.top = p1.y;
        rc.right = p2.x;
        rc.bottom = p2.y;
        ClipCursor(&rc);
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        m_driverType = D3D_DRIVER_TYPE_NULL;
        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
        };
        UINT numDriverTypes = ARRAYSIZE(driverTypes);

        m_featureLevel = D3D_FEATURE_LEVEL_11_0;
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };
        UINT numFeatureLevels = ARRAYSIZE(featureLevels);

        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
        {
            m_driverType = driverTypes[driverTypeIndex];
            hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &m_featureLevel, m_immediateContext.GetAddressOf());

            if (hr == E_INVALIDARG)
            {
                hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                    D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &m_featureLevel, m_immediateContext.GetAddressOf());
            }

            if (SUCCEEDED(hr))
                break;
        }

        ComPtr<IDXGIFactory1> dxgiFactory(nullptr);
        {
            ComPtr<IDXGIDevice> dxgiDevice(nullptr);
            if (SUCCEEDED(m_d3dDevice.As(&dxgiDevice)))
            {
                ComPtr<IDXGIAdapter> adapter(nullptr);

                hr = dxgiDevice->GetAdapter(adapter.GetAddressOf());
                if (SUCCEEDED(hr))
                {
                    hr = adapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf()));
                }
            }
        }
        if (FAILED(hr))
            return hr;

        // Create swap chain
        ComPtr<IDXGIFactory2> dxgiFactory2(nullptr);
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later
            if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
            {
                (void)m_immediateContext.As(&m_immediateContext1);
            }

            DXGI_SWAP_CHAIN_DESC1 sd = {};
            sd.Width = width;
            sd.Height = height;
            sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.BufferCount = 1;

            hr = dxgiFactory2->CreateSwapChainForHwnd(m_d3dDevice.Get(), hWnd, &sd, nullptr, nullptr, m_swapChain1.GetAddressOf());
            if (SUCCEEDED(hr))
            {
                hr=m_swapChain1.As(&m_swapChain);
            }
        }
        else
        {
            // DirectX 11.0 systems
            DXGI_SWAP_CHAIN_DESC sd = {};
            sd.BufferCount = 1;
            sd.BufferDesc.Width = width;
            sd.BufferDesc.Height = height;
            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator = 60;
            sd.BufferDesc.RefreshRate.Denominator = 1;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.OutputWindow = hWnd;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.Windowed = TRUE;
            hr = dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &sd, m_swapChain.GetAddressOf());
            
        }
    
        dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);


        if (FAILED(hr))
            return hr;

        // Create a render target view
        ComPtr<ID3D11Texture2D> pBackBuffer=nullptr;

        hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        if (FAILED(hr))
            return hr;

        hr = m_d3dDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
        if (FAILED(hr))
            return hr;

        D3D11_TEXTURE2D_DESC descDepth = {};
        descDepth.Width = (int)width;
        descDepth.Height = (int)height;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;

        m_d3dDevice->CreateTexture2D(&descDepth, nullptr, m_depthStencil.GetAddressOf());
        if (FAILED(hr))
            return hr;

        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        hr = m_d3dDevice->CreateDepthStencilView(m_depthStencil.Get(), &descDSV, &m_depthStencilView);
        if (FAILED(hr))
            return hr;

        m_immediateContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

        // Setup the viewport
        D3D11_VIEWPORT vp;
        vp.Width = (FLOAT)(width);
        vp.Height = (FLOAT)(height);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;

        m_immediateContext->RSSetViewports(1, &vp);

        std::unordered_map<PCWSTR, std::shared_ptr<VertexShader>>::iterator mVertexShaderIter;
        for (mVertexShaderIter = m_vertexShaders.begin(); mVertexShaderIter != m_vertexShaders.end(); ++mVertexShaderIter)
        {
            mVertexShaderIter->second->Initialize(m_d3dDevice.Get());
        }

        std::unordered_map<PCWSTR, std::shared_ptr<PixelShader>>::iterator mPixelShaderIter;
        for (mPixelShaderIter = m_pixelShaders.begin(); mPixelShaderIter != m_pixelShaders.end(); ++mPixelShaderIter)
        {
            mPixelShaderIter->second->Initialize(m_d3dDevice.Get());
        }

        std::unordered_map<PCWSTR, std::shared_ptr<Renderable>>::iterator renderableIter;
        for (renderableIter = m_renderables.begin(); renderableIter != m_renderables.end(); ++renderableIter)
        {
            renderableIter->second->Initialize(m_d3dDevice.Get(), m_immediateContext.Get());
        }

        m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, (float)(width) / (float)(height), 0.01f, 100.0f);

        return hr;
    }
    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  Method:   Renderer::AddRenderable
  Summary:  Add a renderable object and initialize the object
  Args:     PCWSTR pszRenderableName
              Key of the renderable object
            const std::shared_ptr<Renderable>& renderable
              Unique pointer to the renderable object
  Modifies: [m_renderables].
  Returns:  HRESULT
              Status code.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
/*--------------------------------------------------------------------
  TODO: Renderer::AddRenderable definition (remove the comment)
--------------------------------------------------------------------*/
    HRESULT Renderer::AddRenderable(_In_ PCWSTR pszRenderableName, _In_ const std::shared_ptr<Renderable>& renderable)
    {
        std::unordered_map<PCWSTR, std::shared_ptr<Renderable>>::iterator renderableIter;
        renderableIter = m_renderables.find(pszRenderableName);
        if (renderableIter == m_renderables.end())
        {
            m_renderables.insert(std::make_pair(pszRenderableName, renderable));
            return S_OK;
        }
        else
        {
            return E_FAIL;
        }
    }
    
/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:   Renderer::AddVertexShader
Summary:  Add the vertex shader into the renderer
Args:     PCWSTR pszVertexShaderName
            Key of the vertex shader
          const std::shared_ptr<VertexShader>&
            Vertex shader to add
Modifies: [m_vertexShaders].
Returns:  HRESULT
            Status code
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
/*--------------------------------------------------------------------
  TODO: Renderer::AddVertexShader definition (remove the comment)
--------------------------------------------------------------------*/
    HRESULT Renderer::AddVertexShader(_In_ PCWSTR pszVertexShaderName, _In_ const std::shared_ptr<VertexShader>& vertexShader)
    {
        
        std::unordered_map<PCWSTR, std::shared_ptr<VertexShader>>::iterator mVertexShaderIter;
        mVertexShaderIter = m_vertexShaders.find(pszVertexShaderName);
        if (mVertexShaderIter == m_vertexShaders.end())
        {
            m_vertexShaders.insert(std::make_pair(pszVertexShaderName, vertexShader));
            return S_OK;
        }
        else
        {
            return E_FAIL;
        }
        


        return S_OK;
    }

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  Method:   Renderer::AddPixelShader
  Summary:  Add the pixel shader into the renderer
  Args:     PCWSTR pszPixelShaderName
              Key of the pixel shader
            const std::shared_ptr<PixelShader>&
              Pixel shader to add
  Modifies: [m_pixelShaders].
  Returns:  HRESULT
              Status code
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
/*--------------------------------------------------------------------
  TODO: Renderer::AddPixelShader definition (remove the comment)
--------------------------------------------------------------------*/
    HRESULT Renderer::AddPixelShader(_In_ PCWSTR pszPixelShaderName, _In_ const std::shared_ptr<PixelShader>& pixelShader)
    {
        
        std::unordered_map<PCWSTR, std::shared_ptr<PixelShader>>::iterator mPixelShaderIter;
        mPixelShaderIter = m_pixelShaders.find(pszPixelShaderName);
        if (mPixelShaderIter == m_pixelShaders.end())
        {
            m_pixelShaders.insert(std::make_pair(pszPixelShaderName, pixelShader));
            return S_OK;
        }
        else
        {
            return E_FAIL;
        }

        return S_OK;
    }
    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
      Method:   Renderer::HandleInput
      Summary:  Add the pixel shader into the renderer and initialize it
      Args:     const DirectionsInput& directions
                  Data structure containing keyboard input data
                const MouseRelativeMovement& mouseRelativeMovement
                  Data structure containing mouse relative input data
      Modifies: [m_camera].
    M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
    void Renderer::HandleInput(_In_ const DirectionsInput& directions, _In_ const MouseRelativeMovement& mouseRelativeMovement, _In_ FLOAT deltaTime)
    {
        m_camera.HandleInput(directions, mouseRelativeMovement, deltaTime);
    }
/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  Method:   Renderer::Update
  Summary:  Update the renderables each frame
  Args:     FLOAT deltaTime
              Time difference of a frame
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
/*--------------------------------------------------------------------
  TODO: Renderer::Update definition (remove the comment)
--------------------------------------------------------------------*/
    void Renderer::Update(_In_ FLOAT deltaTime)
    {
        std::unordered_map<PCWSTR, std::shared_ptr<Renderable>>::iterator renderableIter;
        for (renderableIter = m_renderables.begin(); renderableIter != m_renderables.end(); ++renderableIter)
        {
            renderableIter->second->Update(deltaTime);
        }
    }
    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
      Method:   Renderer::Render

      Summary:  Render the frame
    M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
    /*--------------------------------------------------------------------
      TODO: Renderer::Initialize definition (remove the comment)
    --------------------------------------------------------------------*/
    void Renderer::Render()
    {
        m_immediateContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::MidnightBlue);    
        m_immediateContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

        for (auto& each : m_renderables)
        {
            std::shared_ptr<Renderable> renderable = each.second;
            UINT stride = sizeof(SimpleVertex);
            UINT offset = 0;
            m_immediateContext->IASetVertexBuffers(0, 1, renderable->GetVertexBuffer().GetAddressOf(), &stride, &offset);
            m_immediateContext->IASetIndexBuffer(renderable->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
            m_immediateContext->IASetInputLayout(renderable->GetVertexLayout().Get());
            ConstantBuffer cb = {
                .World = XMMatrixTranspose(renderable->GetWorldMatrix()),
                .View = XMMatrixTranspose(m_camera.GetView()),
                .Projection = XMMatrixTranspose(m_projection)
            };
            m_immediateContext->UpdateSubresource(renderable->GetConstantBuffer().Get(), 0, NULL, &cb, 0, 0);
            m_immediateContext->VSSetShader(renderable->GetVertexShader().Get(), nullptr, 0);
            m_immediateContext->VSSetConstantBuffers(0, 1, renderable->GetConstantBuffer().GetAddressOf());
            m_immediateContext->PSSetShader(renderable->GetPixelShader().Get(), nullptr, 0);
            m_immediateContext->DrawIndexed(renderable->GetNumIndices(), 0, 0);
        }
        m_swapChain->Present(0, 0);
    }

    /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
    Method:   Renderer::SetVertexShaderOfRenderable
    Summary:  Sets the vertex shader for a renderable
    Args:     PCWSTR pszRenderableName
                Key of the renderable
              PCWSTR pszVertexShaderName
                Key of the vertex shader
    Modifies: [m_renderables].
    Returns:  HRESULT
                Status code
  M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
  /*--------------------------------------------------------------------
    TODO: Renderer::SetVertexShaderOfRenderable definition (remove the comment)
  --------------------------------------------------------------------*/
    HRESULT Renderer::SetVertexShaderOfRenderable(_In_ PCWSTR pszRenderableName, _In_ PCWSTR pszVertexShaderName)
    {
        
        std::unordered_map<PCWSTR, std::shared_ptr<Renderable>>::iterator renderableIter;
        renderableIter = m_renderables.find(pszRenderableName);
        if (renderableIter == m_renderables.end())
        {
            return E_FAIL;
        }
        std::unordered_map<PCWSTR, std::shared_ptr<VertexShader>>::iterator mVertexShaderIter;
        mVertexShaderIter = m_vertexShaders.find(pszVertexShaderName);
        if (mVertexShaderIter == m_vertexShaders.end())
        {
            return E_FAIL;
        }
        renderableIter->second->SetVertexShader(mVertexShaderIter->second);
        return S_OK;


    }
  /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
    Method:   Renderer::SetPixelShaderOfRenderable
    Summary:  Sets the pixel shader for a renderable
    Args:     PCWSTR pszRenderableName
                Key of the renderable
              PCWSTR pszPixelShaderName
                Key of the pixel shader
    Modifies: [m_renderables].
    Returns:  HRESULT
                Status code
  M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
  /*--------------------------------------------------------------------
    TODO: Renderer::SetPixelShaderOfRenderable definition (remove the comment)
  --------------------------------------------------------------------*/
    HRESULT Renderer::SetPixelShaderOfRenderable(_In_ PCWSTR pszRenderableName, _In_ PCWSTR pszPixelShaderName)
    {
        
        std::unordered_map<PCWSTR, std::shared_ptr<Renderable>>::iterator renderableIter;
        renderableIter = m_renderables.find(pszRenderableName);
        if (renderableIter == m_renderables.end())
        {
            return E_FAIL;
        }
        std::unordered_map<PCWSTR, std::shared_ptr<PixelShader>>::iterator mPixelShaderIter;
        mPixelShaderIter = m_pixelShaders.find(pszPixelShaderName);
        if (mPixelShaderIter == m_pixelShaders.end())
        {
            return E_FAIL;
        }
        renderableIter->second->SetPixelShader(mPixelShaderIter->second);
        return S_OK;
        

    }
  /*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
    Method:   Renderer::GetDriverType
    Summary:  Returns the Direct3D driver type
    Returns:  D3D_DRIVER_TYPE
                The Direct3D driver type used
  M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
  /*--------------------------------------------------------------------
    TODO: Renderer::GetDriverType definition (remove the comment)
  --------------------------------------------------------------------*/
    D3D_DRIVER_TYPE Renderer::GetDriverType() const
    {
        return m_driverType;
    }
}