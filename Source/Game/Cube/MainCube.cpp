#include"Cube/MainCube.h"

void MainCube::Update(_In_ FLOAT deltaTime)
{
	m_world = XMMatrixRotationY(deltaTime);
}