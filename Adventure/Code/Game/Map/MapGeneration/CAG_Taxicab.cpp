#include "Game/Map/MapGeneration/CAG_Taxicab.hpp"

#include "Game/Map/Tile/TileDefinition.hpp"

#include "Engine/Core/Math/Primatives/IntVec2.hpp"

bool CAG_Taxicab::PositionMatch( const IntVec2& position, const Map& mapToMutate )
{
    int intRadius = roundi( m_Radius );
    for ( int xRange = -intRadius; xRange <= intRadius; ++xRange )
    {
        for ( int yRange = -intRadius; yRange <= intRadius; ++yRange )
        {
            if ( xRange == 0 && yRange == 0 ) { continue; }

            IntVec2 offset = IntVec2( xRange, yRange );
            if ( IntVec2::GetTaxicabDistance( position, position + offset ) <= intRadius )
            {
                if ( CheckPositionAgainst( position + offset, m_Neighbor, mapToMutate ) ) { return true; }
            }
        }
    }

    return false;
}

CAG_Taxicab::CAG_Taxicab( const XmlElement& element )
    : CellularAutomataGeneration( element )
{
    m_CaType = CellularAutomataType::TAXICAB;

    m_Neighbor = &ParseXmlAttribute( element, "neighbor", *TileDefinition::DefaultDefinition );
}
