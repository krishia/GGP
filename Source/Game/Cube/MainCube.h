#pragma once
#include "Cube/BaseCube.h"

class MainCube  : public BaseCube
{
public:
	MainCube() = default;
	MainCube(const std::filesystem::path& textureFilePath);
	~MainCube() = default;
	void Update(_In_ FLOAT deltaTime) override;
};