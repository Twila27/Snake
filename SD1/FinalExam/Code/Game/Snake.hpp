#pragma once

#include <vector>
#include "Game/Entity2D.hpp"
#include "Game/GameCommon.hpp"

class Snake
{
public:
	Snake();
	~Snake();

	Entity2D* m_head;
	std::vector< Entity2D* > m_tail;
	SnakeDirection m_forwardDirection;
	float m_movementCounter;
	static const float SECONDS_PER_MOVE;
	static float CURRENT_MAX_LENGTH;
	static const float INITIAL_MAX_LENGTH;
	bool m_isAlive;

	void Update( float deltaSeconds );
	void Render();
	void Die() { m_isAlive = false; m_head->Die(); }
	void Grow() { Snake::CURRENT_MAX_LENGTH += 5; }
};