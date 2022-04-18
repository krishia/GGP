#include "Cube/atomic.h"

atomic::atomic(const std::filesystem::path& textureFilePath)
	: BaseCube(textureFilePath)
{

}
void atomic::Update(_In_ FLOAT deltaTime)
{
	XMMATRIX spin = XMMatrixRotationX(-deltaTime);
	XMMATRIX orbit = XMMatrixRotationZ(-deltaTime * 2.0f);
	XMMATRIX translate = XMMatrixTranslation(-6.0f, 0.0f, 0.0f);
	XMMATRIX scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);

	m_world = scale * spin * translate * orbit;
}