#include"Cube/MainCube.h"

MainCube::MainCube(const std::filesystem::path& textureFilePath)
	: BaseCube(textureFilePath)
{

}

void MainCube::Update(_In_ FLOAT deltaTime)
{
	m_world = XMMatrixRotationY(deltaTime);
}