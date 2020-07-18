#include "Map.hpp"

#include "Game/GlobalSystems.hpp"
#include "Game/Entity/Actor2D.hpp"
#include "Game/Map/MapDefinition.hpp"
#include "Game/Entity/ActorDefinition.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/VertexTypes/VertexMaster.hpp"
#include "Engine/Core/Math/RandomNumberGenerator.hpp"
#include "Engine/Core/Math/Primatives/AABB2.hpp"
#include "Engine/Core/Math/Primatives/Disc.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/Mesh/MeshUtils.hpp"
#include "Engine/Renderer/Sprite/SpriteSheet.hpp"

STATIC Map* Map::CreateMapFromDefinition( Game* gameInstance,
                                          World* worldInstance,
                                          const std::string& mapName,
                                          MapDefinition& mapDefinition )
{
    Map* newMap = new Map( gameInstance, worldInstance, mapName, mapDefinition );
    newMap->Create();
    return newMap;
}

void Map::Create()
{
    unsigned int mapSeed = g_GlobalSeedGenerator->RandomInt32();
    g_Console->Log( LOG_VERBOSE, Stringf( "Map.Create: Creating \"%s\" with seed %x",
                                          m_MapDefinition.GetMapDefName().c_str(),
                                          mapSeed
                                        )
                  );

    m_Rng = new RandomNumberGenerator( mapSeed );

    m_MapSize = m_MapDefinition.GetMapDefSize();

    size_t mapSizeIndexes = m_MapSize.x * m_MapSize.y;
    m_Tiles.reserve( mapSizeIndexes );
    m_TileTags.resize( mapSizeIndexes );

    // Fill map with default tiles
    for( int tileIndex = 0; tileIndex < mapSizeIndexes; ++tileIndex )
    {
        int tileCoordX = tileIndex % m_MapSize.x;
        int tileCoordY = tileIndex / m_MapSize.x;
        IntVec2 tileCoords = IntVec2( tileCoordX, tileCoordY );
        m_Tiles.push_back( Tile( tileCoords, &m_MapDefinition.GetMapDefDefualtFillTile() ) );
    }

    // Fill horizontal edges
    for( int tileIndex = 0; tileIndex < m_MapSize.x; ++tileIndex )
    {
        size_t topEdgeIndex = mapSizeIndexes - 1 - tileIndex;
        m_Tiles[ tileIndex ].SetTileDefinition( &m_MapDefinition.GetMapDefBoundaryTile() );
        m_Tiles[ topEdgeIndex ].SetTileDefinition( &m_MapDefinition.GetMapDefBoundaryTile() );
    }
    // Fill Vertical edges
    for( int tileIndex = 1; tileIndex < m_MapSize.y - 1; ++tileIndex )
    {
        size_t leftEdgeTiles = tileIndex * m_MapSize.x;
        size_t rightEdgeTiles = leftEdgeTiles + m_MapSize.x - 1;
        m_Tiles[ leftEdgeTiles ].SetTileDefinition( &m_MapDefinition.GetMapDefBoundaryTile() );
        m_Tiles[ rightEdgeTiles ].SetTileDefinition( &m_MapDefinition.GetMapDefBoundaryTile() );
    }

    m_MapDefinition.RunGenerationSteps( *this, *m_Rng );

    m_Player = new Actor2D( m_GameInstance,
                            this,
                            "Player",
                            ActorDefinition::GetFromName( "Villager" ),
                            Vec2( 3.f, 3.f ),
                            0.f,
                            Vec2::ONE,
                            0
                          );
    m_Entites.push_back( m_Player );

    int extraEntites = m_Rng->IntInRange( 3, 10 );
    for( int entityIndex = 0; entityIndex < extraEntites; ++entityIndex )
    {
        Vec2 random = Vec2( m_Rng->FloatInRange( 1.f, static_cast<float>(m_MapSize.x) - 1.f ),
                            m_Rng->FloatInRange( 1.f, static_cast<float>(m_MapSize.y) - 1.f )
                          );
        Entity2D* entity = new Actor2D( m_GameInstance,
                                        this,
                                        "Mermaid",
                                        ActorDefinition::GetFromName( "Mermaid" ),
                                        random
                                      );

        m_Entites.push_back( entity );
    }

    extraEntites = m_Rng->IntInRange( 3, 10 );
    for( int entityIndex = 0; entityIndex < extraEntites; ++entityIndex )
    {
        Vec2 random = Vec2( m_Rng->FloatInRange( 1.f, static_cast<float>(m_MapSize.x) - 1.f ),
                            m_Rng->FloatInRange( 1.f, static_cast<float>(m_MapSize.y) - 1.f )
                          );
        Entity2D* entity = new Actor2D( m_GameInstance,
                                        this,
                                        "FlowerGuy",
                                        ActorDefinition::GetFromName( "FlowerGuy" ),
                                        random
                                      );

        m_Entites.push_back( entity );
    }
}

void Map::Update( float deltaSeoncds )
{
    UNUSED( deltaSeoncds );

    for( Entity2D* entity : m_Entites )
    {
        entity->Update( deltaSeoncds );
    }

    HandleEntityVsEntity();
    HandleTileVsEntities();
}

void Map::Render() const
{
    RenderTiles();

    for( const Entity2D* entity : m_Entites )
    {
        entity->Render();
    }
}

void Map::DebugRender() const
{
    for( const Entity2D* entity : m_Entites )
    {
        entity->DebugRender();
    }
}

void Map::Destroy()
{
    m_Tiles.clear();
    m_TileTags.clear();
    m_Entites.clear();
    m_Player = nullptr;
}

const AABB2 Map::GetMapBounds() const
{
    return AABB2( Vec2::ZERO, static_cast<Vec2>(m_MapSize) );
}

Map::Map( Game* gameInstance,
          World* worldInstance,
          const std::string& mapName,
          MapDefinition& mapDefinition )
    : m_GameInstance( gameInstance )
  , m_WorldInstance( worldInstance )
  , m_MapName( mapName )
  , m_MapDefinition( mapDefinition )
{
}

bool Map::IsValidTilePos( const IntVec2& testPos ) const
{
    if( testPos.x < 0 || testPos.y < 0 ) { return false; }
    if( testPos.x > m_MapSize.x - 1 || testPos.y > m_MapSize.y - 1 ) { return false; }
    return true;
}

const IntVec2 Map::GetTilePositionFromWorldCoords( const Vec2& worldCoords ) const
{
    return IntVec2( (int)worldCoords.x, (int)worldCoords.y );
}

Tile* Map::GetTileFromPosition( const IntVec2& position )
{
    if( position.x < 0 || position.y < 0 ) { return nullptr; }
    if( position.x > m_MapSize.x - 1 || position.y > m_MapSize.y - 1 ) { return nullptr; }

    int index = GetTileIndexFromPosition( position );
    return &m_Tiles.at( index );
}

const Tile* Map::GetTileFromPosition( const IntVec2& position ) const
{
    if( position.x < 0 || position.y < 0 ) { return nullptr; }
    if( position.x > m_MapSize.x - 1 || position.y > m_MapSize.y - 1 ) { return nullptr; }

    int index = GetTileIndexFromPosition( position );
    return &m_Tiles.at( index );
}

int Map::GetTileIndexFromPosition( const IntVec2& position ) const
{
    return (position.y * m_MapSize.x) + position.x;
}

Tags* Map::GetTileTagsFromPosition( const IntVec2& position )
{
    if( position.x < 0 || position.y < 0 ) { return nullptr; }
    if( position.x > m_MapSize.x - 1 || position.y > m_MapSize.y - 1 ) { return nullptr; }

    return &m_TileTags.at( GetTileIndexFromPosition( position ) );
}

const Tags* Map::GetTileTagsFromPosition( const IntVec2& position ) const
{
    if( position.x < 0 || position.y < 0 ) { return nullptr; }
    if( position.x > m_MapSize.x - 1 || position.y > m_MapSize.y - 1 ) { return nullptr; }

    return &m_TileTags.at( GetTileIndexFromPosition( position ) );
}

const Entity2D* Map::DoesPointOverlapEntity( const Vec2& point ) const
{
    for( Entity2D* entity : m_Entites )
    {
        if( entity->GetEntityPhysicsDisc().IsPointInside( point ) )
        {
            return entity;
        }
    }

    return nullptr;
}

void Map::Regenerate()
{
    Destroy();
    Create();
}

void Map::HandleEntityVsEntity()
{
    for( int entity1Index = 0; entity1Index < m_Entites.size(); ++entity1Index )
    {
        Entity2D& entity1 = *m_Entites.at( entity1Index );
        for( int entity2Index = 0; entity2Index < m_Entites.size(); ++entity2Index )
        {
            if( entity1Index == entity2Index ) { continue; }

            Entity2D& entity2 = *m_Entites.at( entity2Index );

            if( DoDiscsOverlap( entity1.GetEntityPhysicsDisc(), entity2.GetEntityPhysicsDisc() ) )
            {
                Disc entity1Disc = entity1.GetEntityPhysicsDisc();
                Disc entity2Disc = entity2.GetEntityPhysicsDisc();
                Disc::PushDiscMobileOutOfDiscMobile( entity1Disc, entity2Disc );
                entity1.SetPosition( entity1Disc.center );
                entity2.SetPosition( entity2Disc.center );
            }
        }
    }
}

void Map::HandleTileVsEntities()
{
    static IntVec2 bottomCenter = IntVec2( 0, -1 );
    static IntVec2 rightCenter = IntVec2( 1, 0 );
    static IntVec2 topCenter = IntVec2( 0, 1 );
    static IntVec2 leftCenter = IntVec2( -1, 0 );
    static IntVec2 bottomLeft = IntVec2( -1, -1 );
    static IntVec2 bottomRight = IntVec2( -1, 1 );
    static IntVec2 topRight = IntVec2( 1, 1 );
    static IntVec2 topLeft = IntVec2( 1, -1 );
    for( Entity2D* entity : m_Entites )
    {
        IntVec2 entityTilePosition = GetTilePositionFromWorldCoords( entity->GetPosition() );

        if( IsValidTilePos( entityTilePosition + bottomCenter ) )
        {
            HandleTileVsEntity( *GetTileFromPosition( entityTilePosition + bottomCenter ), *entity );
        }
        if( IsValidTilePos( entityTilePosition + rightCenter ) )
        {
            HandleTileVsEntity( *GetTileFromPosition( entityTilePosition + rightCenter ), *entity );
        }
        if( IsValidTilePos( entityTilePosition + topCenter ) )
        {
            HandleTileVsEntity( *GetTileFromPosition( entityTilePosition + topCenter ), *entity );
        }
        if( IsValidTilePos( entityTilePosition + leftCenter ) )
        {
            HandleTileVsEntity( *GetTileFromPosition( entityTilePosition + leftCenter ), *entity );
        }
        if( IsValidTilePos( entityTilePosition ) )
        {
            HandleTileVsEntity( *GetTileFromPosition( entityTilePosition ), *entity );
        }
        if( IsValidTilePos( entityTilePosition + bottomLeft ) )
        {
            HandleTileVsEntity( *GetTileFromPosition( entityTilePosition + bottomLeft ), *entity );
        }
        if( IsValidTilePos( entityTilePosition + bottomRight ) )
        {
            HandleTileVsEntity( *GetTileFromPosition( entityTilePosition + bottomRight ), *entity );
        }
        if( IsValidTilePos( entityTilePosition + topRight ) )
        {
            HandleTileVsEntity( *GetTileFromPosition( entityTilePosition + topRight ), *entity );
        }
        if( IsValidTilePos( entityTilePosition + topLeft ) )
        {
            HandleTileVsEntity( *GetTileFromPosition( entityTilePosition + topLeft ), *entity );
        }
    }
}

void Map::HandleTileVsEntity( const Tile& tile, Entity2D& entity )
{
    if( DoesTilePushEntity( tile, entity ) )
    {
        PushEntityOutOfTile( entity, tile );
    }
}

bool Map::DoesTilePushEntity( const Tile& tile, const Entity2D& entity ) const
{
    if( tile.DoesTileAllowWalk() && entity.CanWalk() ) { return false; }
    if( tile.DoesTileAllowFly() && entity.CanFly() ) { return false; }
    if( tile.DoesTileAllowSwim() && entity.CanSwim() ) { return false; }

    return true;
}

void Map::PushEntityOutOfTile( Entity2D& pushed, const Tile& tile )
{
    const AABB2& tileBounds = tile.GetTileBounds();
    Entity2D::PushEntityOutOf( pushed, tileBounds );
}

void Map::RenderTiles() const
{
    std::vector<VertexMaster> tileVisual;

    const SpriteSheet* spriteSheetUsed = nullptr;
    for( int tileIndex = 0; tileIndex < m_Tiles.size(); ++tileIndex )
    {
        const Tile& currentTile = m_Tiles.at( tileIndex );
        if( spriteSheetUsed == nullptr )
        {
            spriteSheetUsed = &currentTile.GetTileSpriteDefinition().GetSpriteSheet();
        }
        AddTileToVector( tileVisual, currentTile );
    }

    if( spriteSheetUsed )
    {
        g_Renderer->BindTexture( &spriteSheetUsed->GetTexture() );
        g_Renderer->DrawVertexArray( tileVisual );
    }
    else
    {
        g_Console->Log( LOG_ERROR, Stringf( "Map:RenderTiles - No sprite sheet texture found" ) );
    }
}

void Map::AddTileToVector( std::vector<VertexMaster>& tileVisual,
                           const Tile& tileToAdd ) const
{
    const AABB2& tileUVs = tileToAdd.GetTileUVs();
    AppendAABB2( tileVisual,
                 tileToAdd.GetTileBounds(),
                 tileToAdd.GetTileTint(),
                 tileUVs.mins,
                 tileUVs.maxs
               );
}
