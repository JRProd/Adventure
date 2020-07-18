#pragma once

#include <string>

#include "Engine/Core/Math/Primatives/IntVec2.hpp"
#include "Engine/Core/Math/Primatives/AABB2.hpp"
struct Rgba8;
class SpriteDefinition;

class TileDefinition;

class Tile
{
public:
    Tile() = default;
    Tile( const IntVec2& tileCoords, 
          TileDefinition* tileDefinition );

    //-------------------------------------------------------------------------
    // Tile Accessors (const)
    const IntVec2 GetTileCoords() const { return m_TileCoords; }
    const TileDefinition& GetTileDefinition() const { return *m_TileDefinition; }
    const AABB2 GetTileBounds() const;

    //-------------------------------------------------------------------------
    // Tile Definition Accessors through Tile (const)
    const std::string GetTileName() const;
    const SpriteDefinition GetTileSpriteDefinition() const;
    const Rgba8 GetTileTint() const;
    const AABB2 GetTileUVs() const;
    bool DoesTileAllowWalk() const;
    bool DoesTileAllowFly() const;
    bool DoesTileAllowSwim() const;
    bool DoesTileAllowSight() const;
    bool DoesTileAllowProjectile() const;

    //-------------------------------------------------------------------------
    // Tile Mutators (non-const)
    void SetTileDefinition( TileDefinition* newTileDefinition );

private:
    IntVec2 m_TileCoords = IntVec2::ZERO;
    TileDefinition* m_TileDefinition = nullptr;
};