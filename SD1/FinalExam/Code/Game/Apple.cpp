#include "Game/Apple.hpp"

#include "Game/TheApp.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/TheRenderer.hpp"

Apple::Apple()
	: Entity2D( "" )
	, m_wasJustHit( false )
{
	m_position = Vector2( 625.f + ( SNAKE_TILE_SIZE / 2.f ), g_theApp->m_screenHeight - 75.f - ( SNAKE_TILE_SIZE / 2.f ) );
}

Apple::~Apple()
{

}

void Apple::Render()
{
	g_theRenderer->DrawShadedAABB( m_renderBounds, Rgba( 0.f, 1.f, 0.f ), Rgba( 0.f, .7f, 0.f ), Rgba( 0.f, .7f, 0.f ), Rgba( 0.f, 0.33f, 0.f ) );
}

void Apple::Update( float deltaSeconds )
{
	Entity2D::Update( deltaSeconds );
}

