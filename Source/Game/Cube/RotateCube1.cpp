#include "Cube/RotateCube1.h"

void RotateCube1::Update(_In_ FLOAT deltaTime)
{
	XMMATRIX spin = XMMatrixRotationZ(-deltaTime);
	XMMATRIX orbit = XMMatrixRotationY(-deltaTime * 2.0f);
	XMMATRIX translate = XMMatrixTranslation(-4.0f, 0.0f, 0.0f);
	XMMATRIX scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);

	m_world = scale * spin * translate * orbit;
}