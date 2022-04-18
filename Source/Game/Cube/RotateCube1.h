#pragma once
#include "Cube/BaseCube.h"

class RotateCube1 : public BaseCube
{
public:
	RotateCube1() = default;
	RotateCube1(const std::filesystem::path& textureFilePath);
	~RotateCube1() = default;
	void Update(_In_ FLOAT deltaTime) override;
};
