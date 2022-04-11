#pragma once
#include "Cube/BaseCube.h"

class atomic : public BaseCube
{
public:
	atomic() = default;
	~atomic() = default;
	void Update(_In_ FLOAT deltaTime) override;
};