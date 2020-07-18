#include "Game.hpp"

#include "Game/GlobalSystems.hpp"
#include "Game/World.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Entity/Entity2D.hpp"
#include "Game/Map/MapDefinition.hpp"
#include "Game/Map/Tile/TileDefinition.hpp"
#include "Game/Entity/ActorDefinition.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/VertexTypes/VertexMaster.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input//InputSystem.hpp"
#include "Engine/Core/Math/RandomNumberGenerator.hpp"
#include "Engine/Core/Math/Primatives/IntVec2.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Engine/Renderer/Mesh/MeshUtils.hpp"

bool g_DebugMode = false;

BitmapFont* g_DefaultFont = nullptr;
RandomNumberGenerator* g_ActorBehaviorRng = nullptr;

//-----------------------------------------------------------------------------
Game::Game()
{
}

//-----------------------------------------------------------------------------
Game::~Game()
{
}

bool Game::EventTestFunction( EventArgs* args )
{
    g_Console->Log( LOG_LOG, Stringf( "Event was just pressed" ) );
    return false;
}

//-----------------------------------------------------------------------------
void Game::Startup()
{
    g_ActorBehaviorRng = new RandomNumberGenerator();
    m_Rng = new RandomNumberGenerator();

    g_DefaultFont = g_Renderer->CreateOrGetBitmapFontFromFile( "Data/Fonts/SquirrelFixedFont" );
    g_Console->SetConsoleText( g_DefaultFont );

    m_UICamera = new Camera(g_Renderer);
    m_UICamera->SetProjectionOrthographic( AABB2( Vec2( 0.f, 0.f ),
                                     Vec2( 1920.f, 1080.f ) ) );
    m_UICamera->SetColorTarget( g_Renderer->GetBackBuffer() );

    m_GameCamera = new Camera(g_Renderer);
    float gameViewAspect = g_GameConfigBlackboard.GetValue( "clientAspect", 1.777f );
    float gameViewHeight = g_GameConfigBlackboard.GetValue( "gameMaxHeight", 9.0f );
    m_GameCamera->SetProjectionOrthographic( AABB2( Vec2( 0.f, 0.f ),
                                       Vec2( gameViewHeight * gameViewAspect,
                                             gameViewHeight ) ) );
    m_GameCamera->SetClearMode( CLEAR_COLOR_BIT );
    m_GameCamera->SetColorTarget( g_Renderer->GetBackBuffer() );

    g_Console->Log( LOG_INFO, "Game is starting up" );

    g_Console->Log( LOG_VERBOSE, "Game is loading definitions" );
    XmlDocument tileDocument;
    tileDocument.LoadFile( "Data/Gameplay/TileDefinitions.xml" );
    TileDefinition::PopulateFromXmlElement( *tileDocument.FirstChildElement() );

    XmlDocument mapDocument;
    mapDocument.LoadFile( "Data/Gameplay/MapDefinitions.xml" );
    MapDefinition::PopulateFromXmlElement( *mapDocument.FirstChildElement() );

    XmlDocument actorDocument;
    actorDocument.LoadFile( "Data/Gameplay/Entity/ActorDefinitions.xml" );
    ActorDefinition::PopulateFromXmlElement( *actorDocument.FirstChildElement() );

    m_CurrentWorld = new World();
    m_CurrentWorld->Create();

    AABB2 test = AABB2::CreateFromString( "(1.0, 10.0),(434.3,1)" );
}

//-----------------------------------------------------------------------------
void Game::Shutdown()
{
    m_CurrentWorld->Destory();
    delete m_CurrentWorld;
    m_CurrentWorld = nullptr;

    delete m_GameCamera;
    m_GameCamera = nullptr;

    delete m_UICamera;
    m_UICamera = nullptr;

    delete m_Rng;
    m_Rng = nullptr;

    delete g_ActorBehaviorRng;
    g_ActorBehaviorRng = nullptr;
}

//-----------------------------------------------------------------------------

RandomNumberGenerator* Game::GetRng()
{
    return m_Rng;
}

//-----------------------------------------------------------------------------
void Game::Update( float deltaSeconds )
{
    deltaSeconds = UpdateDeltaSeconds( deltaSeconds );
    m_GameTime += deltaSeconds;

    HandleUserInput();

    if ( m_CurrentWorld )
    {
        m_CurrentWorld->Update( deltaSeconds );

        UpdateCameraPositionToPlayerPosition();
    }
}

//-----------------------------------------------------------------------------
void Game::Render() const
{
    // Render Game
    g_Renderer->BeginCamera( *m_GameCamera );

    m_CurrentWorld->Render();

    if ( g_DebugMode )
    {
        DebugRender();
    }

    g_Renderer->EndCamera( *m_GameCamera );

    // Render UI
    g_Renderer->BeginCamera( *m_UICamera );

    g_Renderer->BindTexture( nullptr );
    g_Console->Render( *g_Renderer, *m_UICamera );

    if ( g_DebugMode )
    {
        DebugInspector();
    }

    g_Renderer->EndCamera( *m_UICamera );

}

float Game::UpdateDeltaSeconds( float deltaSeconds )
{
    if ( m_IsSlowMo ) { deltaSeconds *= m_SlowMoPercentage; }
    if ( m_IsHyperSpeed ) { deltaSeconds *= m_HyperSpeedPercentage; }
    if ( m_IsPaused ) { deltaSeconds = 0; }
    return deltaSeconds;
}

void Game::HandleUserInput()
{
    if ( g_InputSystem->WasKeyJustPressed( F1 ) )
    {
        g_DebugMode = !g_DebugMode;
    }

    if ( g_InputSystem->WasKeyJustPressed( F2 ) )
    {
        m_FullMapView = !m_FullMapView;
    }

    if ( g_InputSystem->WasKeyJustPressed( 'J' ) )
    {
        g_EventSystem->Subscribe( "TestEvent", EventTestFunction );
    }

    if ( g_InputSystem->WasKeyJustPressed( 'K' ) )
    {
        g_EventSystem->Unsubscribe( "TestEvent", EventTestFunction );
    }

    if ( g_InputSystem->WasKeyJustPressed( 'L' ) )
    {
        g_EventSystem->Publish( "TestEvent" );
    }

    if ( g_InputSystem->IsKeyPressed( 'T' ) && !g_InputSystem->IsKeyPressed( 'Y' ) )
    {
        m_IsSlowMo = true;
    }
    else
    {
        m_IsSlowMo = false;
    }

    if ( g_InputSystem->IsKeyPressed( 'Y' ) && !g_InputSystem->IsKeyPressed( 'T' ) )
    {
        m_IsHyperSpeed = true;
    }
    else
    {
        m_IsHyperSpeed = false;
    }

    if ( g_InputSystem->WasKeyJustPressed( 'P' ) )
    {
        m_IsPaused = !m_IsPaused;
    }
}

void Game::UpdateCameraPositionToPlayerPosition()
{
    float clientAspect = g_GameConfigBlackboard.GetValue( "clientAspect", 1.f );
    if ( m_FullMapView )
    {
        Vec2 mapSize = m_CurrentWorld->GetCurrentMap().GetMapBounds().GetDimensions();
        if ( mapSize.x > (mapSize.y * clientAspect) )
        {
            float clientSlope = 1.f / g_GameConfigBlackboard.GetValue( "clientAspect", 1.f );
            m_GameCamera->SetProjectionOrthographic( AABB2( m_CurrentWorld->GetCurrentMap().GetMapBounds().mins,
                                               Vec2( mapSize.x, mapSize.x * clientSlope ) ) );
        }
        else
        {
            m_GameCamera->SetProjectionOrthographic( AABB2( m_CurrentWorld->GetCurrentMap().GetMapBounds().mins,
                                               Vec2( mapSize.y * clientAspect, mapSize.y ) ) );
        }
    }
    else
    {
        const Entity2D& player = m_CurrentWorld->GetPlayer();

        m_GameCamera->SetProjectionOrthographic( m_TileViewHeight );
        m_GameCamera->SetCameraPosition( static_cast<Vec3>(player.GetPosition()) );
        m_GameCamera->FitCameraInAABB2( m_CurrentWorld->GetCurrentMap().GetMapBounds() );
    }
}


//-----------------------------------------------------------------------------
void Game::DebugRender() const
{
    m_CurrentWorld->DebugRender();
}

void Game::DebugInspector() const
{
    Vec2 boxSize = Vec2( 400.f, 200.f );
    Vec2 normalizedMousePos = g_InputSystem->GetMousePositionNormalized();
    Vec2 mousePosScreenSpace = m_UICamera->GetOrthoView().GetPointAtUV( normalizedMousePos );

    Vec2 bottomLeft = mousePosScreenSpace + Vec2( 50.f, 25.f );
    AABB2 box = AABB2( bottomLeft, bottomLeft + boxSize );
    box.FitWithinBounds( m_UICamera->GetOrthoView() );

    std::vector<VertexMaster> background;
    AppendAABB2( background, box, Rgba8::DARK_GRAY_50 );

    g_Renderer->BindTexture( nullptr );
    g_Renderer->DrawVertexArray( background );

    Vec2 mousePosWorld = m_GameCamera->GetWorldSpaceFromMouse( 0 );
    const Map& map = m_CurrentWorld->GetCurrentMap();

    const Entity2D* entityCollision = map.DoesPointOverlapEntity( mousePosWorld );
    if ( entityCollision )
    {
        std::vector<VertexMaster> textVisual;
        std::string entityName = entityCollision->GetName();

        g_DefaultFont->AddVertsForTextInBox( textVisual, entityName, box, 20.f, Vec2( .05f, .9f ) );

        g_Renderer->BindTexture( g_DefaultFont->GetTexture() );
        g_Renderer->DrawVertexArray( textVisual );
        return;
    }

    IntVec2 tilePosition = map.GetTilePositionFromWorldCoords( mousePosWorld );
    const Tile* tile = map.GetTileFromPosition( tilePosition );
    if ( tile )
    {
        std::vector<VertexMaster> textVisual;
        std::string tileName = tile->GetTileName();
        std::string tilePos = tilePosition.ToString();
        const Tags* tileTags = map.GetTileTagsFromPosition( tilePosition );
        bool canWalk = tile->DoesTileAllowWalk();
        bool canFly = tile->DoesTileAllowFly();
        bool canSwim = tile->DoesTileAllowSwim();
        bool doesAllowProjectiles = tile->DoesTileAllowProjectile();
        bool doesAllowSight = tile->DoesTileAllowSight();

        g_DefaultFont->AddVertsForTextInBox( textVisual, tileName, box, 20.f, Vec2( .05f, .9f ) );
        g_DefaultFont->AddVertsForTextInBox( textVisual, tilePos, box, 20.f, Vec2( .95f, .9f ) );
        g_DefaultFont->AddVertsForTextInBox( textVisual, canWalk ? "Walk Y" : "Walk N", box, 15.f, Vec2( .05f, .75f ) );
        g_DefaultFont->AddVertsForTextInBox( textVisual, canFly ? "Fly Y" : "Fly N", box, 15.f, Vec2( .5f, .75f ) );
        g_DefaultFont->AddVertsForTextInBox( textVisual, canSwim ? "Swim Y" : "Swim N", box, 15.f, Vec2( .95f, .75f ) );
        g_DefaultFont->AddVertsForTextInBox( textVisual, doesAllowProjectiles ? "Projectiles Y" : "Projectiles N", box, 15.f, Vec2( .05f, .65f ) );
        g_DefaultFont->AddVertsForTextInBox( textVisual, doesAllowSight ? "Sight Y" : "Sight N", box, 15.f, Vec2( .95f, .65f ) );

        g_DefaultFont->AddVertsForTextInBox( textVisual, "Tags", box, 15.f, Vec2( .05f, .5f ) );
        std::string tagList = "";
        for ( std::string tag : tileTags->tags )
        {
            tagList += tag + "\n";
        }
        g_DefaultFont->AddVertsForTextInBox( textVisual, tagList, box, 12.f, Vec2( .05f, .2f ) );

        g_Renderer->BindTexture( g_DefaultFont->GetTexture() );
        g_Renderer->DrawVertexArray( textVisual );
        return;
    }
}

//-----------------------------------------------------------------------------
void Game::ErrorRecoverable( const char* message )
{
    if ( m_IsSlowMo )
    {
        m_IsSlowMo = false;
    }
    ERROR_RECOVERABLE( message );
}
