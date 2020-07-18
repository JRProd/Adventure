#include "Tile.hpp"

#include "Game/Map/Tile/TileDefinition.hpp" 

Tile::Tile( const IntVec2& tileCoords, TileDefinition* tileDefinition )
    : m_TileCoords( tileCoords )
    , m_TileDefinition( tileDefinition )
{
}

const AABB2 Tile::GetTileBounds() const
{
    AABB2 tileBound = AABB2::UNIT_BOX;
    tileBound.Translate( static_cast<Vec2>(m_TileCoords) );
    return tileBound;
}

const std::string Tile::GetTileName() const
{
    return m_TileDefinition->GetTileDefName();
}

const SpriteDefinition Tile::GetTileSpriteDefinition() const
{
    return m_TileDefinition->GetTileDefSpriteDefinition();
}

const Rgba8 Tile::GetTileTint() const
{
    return m_TileDefinition->GetTileDefTint();
}

const AABB2 Tile::GetTileUVs() const
{
    return m_TileDefinition->GetTileDefSpriteUVs();
}

bool Tile::DoesTileAllowWalk() const
{
    return m_TileDefinition->DoesTileDefAllowWalk();
}

bool Tile::DoesTileAllowFly() const
{
    return m_TileDefinition->DoesTileDefAllowFly();
}

bool Tile::DoesTileAllowSwim() const
{
    return m_TileDefinition->DoesTileDefAllowSwim();
}

bool Tile::DoesTileAllowSight() const
{
    return m_TileDefinition->DoesTileDefAllowSight();
}

bool Tile::DoesTileAllowProjectile() const
{
    return m_TileDefinition->DoesTileDefAllowProjectile();
}

void Tile::SetTileDefinition( TileDefinition* newTileDefinition )
{
    m_TileDefinition = newTileDefinition;
}
