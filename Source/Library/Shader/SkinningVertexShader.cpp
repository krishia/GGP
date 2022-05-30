#include "Shader/SkinningVertexShader.h"

namespace library
{
    SkinningVertexShader::SkinningVertexShader(_In_ PCWSTR pszFileName, _In_ PCSTR pszEntryPoint, _In_ PCSTR pszShaderModel)
        : VertexShader(pszFileName, pszEntryPoint, pszShaderModel)
    {

    }

    HRESULT SkinningVertexShader::Initialize(_In_ ID3D11Device* pDevice)
    {
        ComPtr<ID3DBlob> vsBlob;
        HRESULT hr = compile(vsBlob.GetAddressOf());
        if (FAILED(hr))
        {
            return hr;
        }

        hr = pDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
        if (FAILED(hr))
        {
            return hr;
        }

        D3D11_INPUT_ELEMENT_DESC aLayouts[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        UINT uNumElements = ARRAYSIZE(aLayouts);

        hr = pDevice->CreateInputLayout(aLayouts, uNumElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), m_vertexLayout.GetAddressOf());

        return hr;
    }
}