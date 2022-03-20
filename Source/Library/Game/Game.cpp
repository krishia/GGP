#include "Game/Game.h"


namespace library
{
    /*--------------------------------------------------------------------
      Global Variables
    --------------------------------------------------------------------*/
    /*--------------------------------------------------------------------
      TODO: Initialize global variables (remove the comment)
    --------------------------------------------------------------------*/
    HINSTANCE g_hInst;
    HWND g_hWnd;
    D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
    ComPtr<ID3D11Device> m_pd3dDevice;
    ComPtr<ID3D11Device1> m_pd3dDevice1;
    ComPtr<ID3D11DeviceContext> m_pd3dDeviceContext;
    ComPtr<ID3D11DeviceContext1> m_pd3dDeviceContext1;
    ComPtr<IDXGISwapChain> m_pDXGISwapChain;
    ComPtr<IDXGISwapChain1> m_pDXGISwapChain1;
    ComPtr<ID3D11Texture2D>        m_pBackBuffer;
    ComPtr<ID3D11RenderTargetView> m_pRenderTarget;
    ComPtr<ID3D11Texture2D>         m_pDepthStencil;
    ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;
    /*--------------------------------------------------------------------
      Forward declarations
    --------------------------------------------------------------------*/
    HRESULT InitWindow(_In_ HINSTANCE hInstance, _In_ INT nCmdShow);
    HRESULT InitDevice();
    void CleanupDevice();
    LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
    void Render();
    /*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      Function: WindowProc

      Summary:  Defines the behavior of the window—its appearance, how
                it interacts with the user, and so forth

      Args:     HWND hWnd
                  Handle to the window
                UINT uMsg
                  Message code
                WPARAM wParam
                  Additional data that pertains to the message
                LPARAM lParam
                  Additional data that pertains to the message

      Returns:  LRESULT
                  Integer value that your program returns to Windows
    -----------------------------------------------------------------F-F*/

    LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
    {
        PAINTSTRUCT ps;
        HDC hdc;

        switch (uMsg)
        {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

            // Note that this tutorial does not handle resizing (WM_SIZE) requests,
            // so we created the window without the resize border.

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        return 0;
    }

    /*--------------------------------------------------------------------
      TODO: Function definitions (remove this comment)
    --------------------------------------------------------------------*/
    HRESULT InitWindow(_In_ HINSTANCE hInstance, _In_ INT nCmdShow)
    {
        if (FAILED(library::InitWindow(hInstance, nCmdShow)))
        {
            return 0;
        }
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WindowProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = L"TutorialWindowClass";
        wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
        if (!RegisterClassEx(&wcex))
            return E_FAIL;

        // Create window
        g_hInst = hInstance;
        RECT rc = { 0, 0, 800, 600 };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        g_hWnd = CreateWindow(L"TutorialWindowClass", L"Direct3D 11 Tutorial 1: Direct3D 11 Basics",
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
            CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, 0, hInstance,
            0);
        if (!g_hWnd)
            return E_FAIL;

        ShowWindow(g_hWnd, nCmdShow);

        return S_OK;
        
    }


    HRESULT InitDevice()
    {
        HRESULT hr = S_OK;

        RECT rc;
        GetClientRect(g_hWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;
        UINT createDeviceFlags = 0;

#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
        };
        UINT numDriverTypes = ARRAYSIZE(driverTypes);

        D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_9_1,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_11_1
        };

        UINT numFeatureLevels = ARRAYSIZE(levels);
        // This flag adds support for surfaces with a color-channel ordering different
        // from the API default. It is required for compatibility with Direct2D.

        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
        {
            g_driverType = driverTypes[driverTypeIndex];
            hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, levels, numFeatureLevels,
                D3D11_SDK_VERSION, &m_pd3dDevice, &g_featureLevel, &m_pd3dDeviceContext);

            if (hr == E_INVALIDARG)
            {
                // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
                hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &levels[1], numFeatureLevels - 1,
                    D3D11_SDK_VERSION, &m_pd3dDevice, &g_featureLevel, &m_pd3dDeviceContext);
            }

            if (SUCCEEDED(hr))
                break;
        }
        if (FAILED(hr))
            return hr;

        ComPtr<IDXGIFactory1> dxgiFactory;
        {
            ComPtr<IDXGIDevice> dxgiDevice;
            hr= m_pd3dDevice.As(&dxgiDevice);
            if (SUCCEEDED(hr))
            {
                ComPtr<IDXGIAdapter> adapter;
                hr = dxgiDevice->GetAdapter(&adapter);
                if (SUCCEEDED(hr))
                {
                    hr = adapter->GetParent(__uuidof(IDXGIFactory1), (void**)&dxgiFactory);
                    adapter->Release();
                }
                dxgiDevice->Release();
            }
        }
        if (FAILED(hr))
            return hr;
  

        
        

        



        ComPtr<IDXGIFactory2> dxgiFactory2;
        dxgiFactory.As(&dxgiFactory2);
        if (dxgiFactory2)
        {
            hr = m_pd3dDevice.As(&m_pd3dDevice1);
            if (SUCCEEDED(hr))
            {
                m_pd3dDeviceContext.As(&m_pd3dDeviceContext1);
            }

            DXGI_SWAP_CHAIN_DESC1 sd = {};
            sd.Width = width;
            sd.Height = height;
            sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.BufferCount = 1;

            hr = dxgiFactory2->CreateSwapChainForHwnd(m_pd3dDevice.Get(), g_hWnd, &sd, nullptr, nullptr, &m_pDXGISwapChain1);
            if (SUCCEEDED(hr))
            {
                hr = m_pDXGISwapChain1.As(&m_pDXGISwapChain);
            }

            dxgiFactory2->Release();
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC sd = {};
            sd.BufferCount = 1;
            sd.BufferDesc.Width = width;
            sd.BufferDesc.Height = height;
            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator = 60;
            sd.BufferDesc.RefreshRate.Denominator = 1;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.OutputWindow = g_hWnd;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.Windowed = TRUE;

            hr = dxgiFactory->CreateSwapChain(m_pd3dDevice.Get(), &sd, &m_pDXGISwapChain);
        }

        dxgiFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER);
        dxgiFactory->Release();

        hr = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&m_pBackBuffer));
        if (FAILED(hr))
            return hr;
        hr=m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer.Get(), nullptr, &m_pRenderTarget);
        if (FAILED(hr))
            return hr;
        m_pBackBuffer->Release();
        m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pRenderTarget, NULL);

        D3D11_VIEWPORT vp;
        vp.Width = (FLOAT)width;
        vp.Height = (FLOAT)height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        m_pd3dDeviceContext->RSSetViewports(1, &vp);

        return S_OK;

    }

    void CleanupDevice()
    {
        if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();

        if (m_pRenderTarget) m_pRenderTarget->Release();
        if (m_pDXGISwapChain1) m_pDXGISwapChain1->Release();
        if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
        if (m_pd3dDeviceContext1) m_pd3dDeviceContext1->Release();
        if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();
        if (m_pd3dDevice1) m_pd3dDevice1->Release();
        if (m_pd3dDevice) m_pd3dDevice->Release();
    }

    void Render()
    {

        m_pd3dDeviceContext->ClearRenderTargetView(m_pRenderTarget.Get(), Colors::MidnightBlue);
        m_pDXGISwapChain->Present(0, 0);
    }

    
}