#pragma once

#include "Game/Entity2D.hpp"


class Apple : public Entity2D
{
public:
	Apple();
	~Apple();
	bool m_wasJustHit;

	void Render() override;
	void Update( float deltaSeconds ) override;
};