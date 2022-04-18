#include "Cube/RotateCube1.h"

RotateCube1::RotateCube1(const std::filesystem::path& textureFilePath)
	: BaseCube(textureFilePath)
{

}
void RotateCube1::Update(_In_ FLOAT deltaTime)
{
	static FLOAT totalTime = 0.0f;
	totalTime += deltaTime;

	static XMMATRIX spin = XMMatrixIdentity();
	spin = XMMatrixRotationZ(-totalTime);

	static XMMATRIX orbit = XMMatrixIdentity();
	orbit = XMMatrixRotationY(-totalTime * 2.0f);

	static XMMATRIX translate = XMMatrixIdentity();
	translate = XMMatrixTranslation(-4.0f, XMScalarSin(totalTime), 0.0f);

	static XMMATRIX scale = XMMatrixScaling(0.5f, 0.5f, 0.5f);

	m_world = scale * spin * translate * orbit;

	
}