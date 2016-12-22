#include "Game/Snake.hpp"


#include "Game/TheApp.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/MathUtils.hpp"

STATIC const float Snake::SECONDS_PER_MOVE = 1.f / 10.f;
STATIC float Snake::CURRENT_MAX_LENGTH = 10.f;

Snake::Snake()
{
	m_head = new Entity2D( "" );
	m_head->SetPosition( 450.f + ( SNAKE_TILE_SIZE / 2.f ), 225.f + ( SNAKE_TILE_SIZE / 2.f ) );
	m_tail.push_back( new Entity2D( "" ) ); //Game starts with 1 tail.
	m_tail[ 0 ]->SetPosition( m_head->GetPosition( ) + Vector2( 0.f, -SNAKE_TILE_SIZE ) );
	m_forwardDirection = UP;
	m_movementCounter = 0.f;
	m_isAlive = true;
}

Snake::~Snake()
{
	delete m_head;
	for ( int i = 0; i < (int)m_tail.size(); i++ ) delete m_tail[ i ];
}

void Snake::Update( float deltaSeconds )
{
	//Entity2D::Update()'s had all movement/rotation stuff commented out, so it just updates age and render bounds now.
	m_head->Update( deltaSeconds );
	for ( int i = 0; i < (int)m_tail.size(); i++ ) m_tail[ i ]->Update( deltaSeconds );

	if ( m_isAlive == false ) return; //Not at top so ageDead gets updated.

	//Update forward direction.
	if ( g_theApp->isKeyDown( VK_UP ) && g_theApp->WasKeyJustPressed( VK_UP ) )
	{
		m_forwardDirection = SnakeDirection::UP;
	}
	else if ( g_theApp->isKeyDown( VK_LEFT ) && g_theApp->WasKeyJustPressed( VK_LEFT ) )
	{
		m_forwardDirection = SnakeDirection::LEFT;
	}
	else if ( g_theApp->isKeyDown( VK_DOWN ) && g_theApp->WasKeyJustPressed( VK_DOWN ) )
	{
		m_forwardDirection = SnakeDirection::DOWN;
	}
	else if ( g_theApp->isKeyDown( VK_RIGHT ) && g_theApp->WasKeyJustPressed( VK_RIGHT ) )
	{
		m_forwardDirection = SnakeDirection::RIGHT;
	}

	//Actual movement below.
	m_movementCounter += deltaSeconds;
	if ( m_movementCounter > Snake::SECONDS_PER_MOVE )
	{
		m_movementCounter = 0.f;

		//Expand tail at start of game per move if shorter than initial max length of 10.
		if ( (int)m_tail.size() < Snake::CURRENT_MAX_LENGTH )
		{
			Entity2D* newTailTile = new Entity2D( "" );
			Vector2 offsetFromLastTailTile;
			switch ( m_forwardDirection )
			{
			case UP: offsetFromLastTailTile = Vector2( 0.f, SNAKE_TILE_SIZE ); break;
			case LEFT: offsetFromLastTailTile = Vector2( -SNAKE_TILE_SIZE, 0.f ); break;
			case DOWN: offsetFromLastTailTile = Vector2( 0.f, -SNAKE_TILE_SIZE ); break;
			case RIGHT:	offsetFromLastTailTile = Vector2( SNAKE_TILE_SIZE, 0.f ); break;
			}
			newTailTile->SetPosition( m_tail.back()->GetPosition() + offsetFromLastTailTile );
			m_tail.push_back( newTailTile );
			newTailTile = nullptr;
		}

		//Update m_tail[i] to be equal to the position of the piece m_tail[i-1] before it, and m_tail[0] equal to m_head.
		Vector2 lastPos;
		for ( int i = (int)m_tail.size() - 1; i >= 0; i-- )
		{
			if ( i == 0 ) m_tail[ i ]->SetPosition( m_head->GetPosition() );
			else m_tail[ i ]->SetPosition( m_tail[ i - 1 ]->GetPosition() );
		}

		//Update head.
		switch ( m_forwardDirection )
		{
		case UP: 
			if ( m_head->GetPosition().y + SNAKE_TILE_SIZE > g_theApp->m_screenHeight ) Die();
			else m_head->SetPosition( m_head->GetPosition() + Vector2( 0.f, SNAKE_TILE_SIZE ) ); 
			break;
		case LEFT:
			if ( m_head->GetPosition().x - SNAKE_TILE_SIZE < 0.f ) Die();
			else m_head->SetPosition( m_head->GetPosition() + Vector2( -SNAKE_TILE_SIZE, 0.f ) ); 
			break;
		case DOWN:
			if ( m_head->GetPosition().y - SNAKE_TILE_SIZE < 0.f ) Die();
			else m_head->SetPosition( m_head->GetPosition() + Vector2( 0.f, -SNAKE_TILE_SIZE ) ); 
			break;
		case RIGHT:
			if ( m_head->GetPosition().x + SNAKE_TILE_SIZE > g_theApp->m_screenWidth ) Die();
			else m_head->SetPosition( m_head->GetPosition() + Vector2( SNAKE_TILE_SIZE, 0.f ) ); 
			break;
		}
	}

	//Head-tail collision.
	for ( auto tailIter = m_tail.begin(); tailIter != m_tail.end(); )
	{
		Entity2D* currentTailTile = *tailIter;
		if ( m_head->DoesOverlap( *currentTailTile ) )
		{
			delete currentTailTile;
			tailIter = m_tail.erase( tailIter ); //HACKY?
			Die();
		}
		else ++tailIter;
	}
}

void Snake::Render()
{
	if ( m_isAlive )
	{
		g_theRenderer->DrawShadedAABB( m_head->m_renderBounds, Rgba( 1.f, 1.f, 1.f ), Rgba( .7f, .7f, .7f ), Rgba( .7f, .7f, .7f ), Rgba( 0.33f, 0.33f, 0.33f ) );
		for ( int i = 0; i < (int)m_tail.size(); i++ ) g_theRenderer->DrawAABB( m_tail[ i ]->m_renderBounds, Rgba( .1f, .4f, 1.f ) );
	}
	else
	{
		float amtAlpha = 1.f * Interpolate( 1.f, 0.f, m_head->GetAgeDeadInSeconds() / SECONDS_BETWEEN_DEATH_AND_EXIT );
		g_theRenderer->DrawShadedAABB( m_head->m_renderBounds, Rgba( 1.f, 0.f, 0.f, amtAlpha ), Rgba( .7f, 0.f, 0.f, amtAlpha ), Rgba( .7f, 0.f, 0.f, amtAlpha ), Rgba( 0.33f, 0.f, 0.f, amtAlpha ) );
		for ( int i = 0; i < (int)m_tail.size(); i++ ) g_theRenderer->DrawAABB( m_tail[ i ]->m_renderBounds, Rgba( 1.f, 0.f, 0.f, amtAlpha ) );
	}
}