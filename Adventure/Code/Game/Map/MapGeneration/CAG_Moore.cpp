#include "Game/Map/MapGeneration/CAG_Moore.hpp"

#include "Game/Map/Tile/TileDefinition.hpp"

#include "Engine/Core/Math/Primatives/IntVec2.hpp"

bool CAG_Moore::PositionMatch( const IntVec2& position, const Map& mapToMutate )
{
    int intRadius = roundi( m_Radius );
    for ( int xRange = -intRadius; xRange <= intRadius; ++xRange )
    {
        for ( int yRange = -intRadius; yRange <= intRadius; ++yRange )
        {
            if ( xRange == 0 && yRange == 0 ) { continue; }

            IntVec2 offset = IntVec2( xRange, yRange );
            if ( CheckPositionAgainst( position + offset, m_Neighbor, mapToMutate ) ) { return true; }
        }
    }

    return false;
}

CAG_Moore::CAG_Moore( const XmlElement& element )
    : CellularAutomataGeneration(element)
{
    m_CaType = CellularAutomataType::MOORE;

    m_Neighbor = &ParseXmlAttribute( element, "neighbor", *TileDefinition::DefaultDefinition );
}
