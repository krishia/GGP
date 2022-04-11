#pragma once
#include "Cube/BaseCube.h"

class MainCube  : public BaseCube
{
public:
	MainCube() = default;
	~MainCube() = default;
	void Update(_In_ FLOAT deltaTime) override;
};