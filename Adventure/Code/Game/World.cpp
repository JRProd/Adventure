#include "World.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"

#include "Game/GlobalSystems.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Map/MapDefinition.hpp"

void World::Create()
{
    g_Console->Log( LOG_INFO, "World is starting up" );

    m_CurrentMap = Map::CreateMapFromDefinition( nullptr, 
                                                 this, 
                                                 "Weird Ruins", 
                                                 MapDefinition::GetFromName( "Island" ) );
}

void World::Update( float deltaSeconds )
{
    if ( g_InputSystem->WasKeyJustPressed( F9 ) )
    {
        RegenerateCurrentMap();
    }

    if ( m_CurrentMap )
    {
        m_CurrentMap->Update( deltaSeconds );
    }
}

void World::Render() const
{
    if ( m_CurrentMap )
    {
        m_CurrentMap->Render();
    }
}

void World::DebugRender() const
{
    if ( m_CurrentMap )
    {
        m_CurrentMap->DebugRender();
    }
}

void World::Destory()
{
}

const Entity2D& World::GetPlayer() const
{
    return m_CurrentMap->GetPlayer();
}

void World::RegenerateCurrentMap()
{
    m_CurrentMap->Regenerate();
}
