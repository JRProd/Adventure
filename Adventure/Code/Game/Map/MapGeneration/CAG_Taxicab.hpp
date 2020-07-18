#pragma once

#include "Game/Map/MapGeneration/CellularAutomataGeneration.hpp"

class TileDefinition;

class CAG_Taxicab: public CellularAutomataGeneration
{
    friend class CellularAutomataGeneration;
public:
    bool PositionMatch( const IntVec2& position, const Map& mapToMutate ) override;

private:
    TileDefinition* m_Neighbor = nullptr;

    CAG_Taxicab( const XmlElement& element );
};