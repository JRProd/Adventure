#pragma once

#include "Game/Map/MapGeneration/CellularAutomataGeneration.hpp"

class TileDefinition;

class CAG_Cardinal: public CellularAutomataGeneration
{
    friend class CellularAutomataGeneration;
public:
    bool PositionMatch( const IntVec2& position, const Map& mapToMutate ) override;

private:
    TileDefinition* m_EastTile = nullptr;
    TileDefinition* m_NorthTile = nullptr;
    TileDefinition* m_WestTile = nullptr;
    TileDefinition* m_SouthTile = nullptr;

    CAG_Cardinal( const XmlElement& element );
};