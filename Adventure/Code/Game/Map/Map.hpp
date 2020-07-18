#pragma once

#include "Game/Map/Tile/Tile.hpp"
#include "Game/Map/MapDefinition.hpp"

#include "Engine/Core/Tags.hpp"
#include "Engine/Core/VertexTypes/VertexMaster.hpp"


#include <string>

class RandomNumberGenerator;

class Game;
class World;
class Entity2D;
class MapDefinition;

typedef std::vector<Tile> Tiles;

class Map
{
public:
    static Map* CreateMapFromDefinition( Game* gameInstance,
                                         World* worldInstance,
                                         const std::string& mapName,
                                         MapDefinition& mapDefinition );

    void Create();
    void Update( float deltaSeoncds );
    void Render() const;
    void DebugRender() const;
    void Destroy();

    //-------------------------------------------------------------------------
    // Map queries
    const Entity2D& GetPlayer() const { return *m_Player; }
    const AABB2 GetMapBounds() const;
    //-------------------------------------------------------------------------
    // Tile queries
    bool IsValidTilePos( const IntVec2& testPos ) const;
    const IntVec2 GetTilePositionFromWorldCoords( const Vec2& worldCoords ) const;
    int GetTileIndexFromPosition( const IntVec2& position ) const;

    //-------------------------------------------------------------------------
    // Tile Mutators
    Tile* GetTileFromPosition( const IntVec2& position );
    const Tile* GetTileFromPosition( const IntVec2& position ) const;
    Tags* GetTileTagsFromPosition( const IntVec2& position );
    const Tags* GetTileTagsFromPosition( const IntVec2& position ) const;

    //-------------------------------------------------------------------------
    // Entity queries
    const Entity2D* DoesPointOverlapEntity( const Vec2& point ) const;

    //-------------------------------------------------------------------------
    // Map Utilties
    void Regenerate();

private:
    Game* m_GameInstance = nullptr;
    World* m_WorldInstance = nullptr;
    RandomNumberGenerator* m_Rng = nullptr;
    MapDefinition& m_MapDefinition;

    std::string m_MapName = "NO_MAP_NAME";
    Tiles m_Tiles;
    std::vector<Tags> m_TileTags;
    std::vector<Entity2D*> m_Entites;
    Entity2D* m_Player = nullptr;


    IntVec2 m_MapSize = IntVec2::ZERO;

    Map( Game* gameInstance,
         World* worldInstance,
         const std::string& mapName,
         MapDefinition& mapDefinition );

    void HandleEntityVsEntity();
    void HandleTileVsEntities();
    void HandleTileVsEntity( const Tile& tile, Entity2D& entity );
    bool DoesTilePushEntity( const Tile& tile, const Entity2D& entity ) const;
    void PushEntityOutOfTile( Entity2D& pushed, const Tile& tile );

    void RenderTiles() const;
    void AddTileToVector( std::vector<VertexMaster>& tileVisual,
                          const Tile& tileToAdd ) const;
};