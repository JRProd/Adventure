#include "Game/Map/MapGeneration/CAG_Circular.hpp"

#include "Game/Map/Tile/TileDefinition.hpp"

#include "Engine/Core/Math/Primatives/IntVec2.hpp"
#include "Engine/Core/Math/Primatives/Vec2.hpp"

bool CAG_Circular::PositionMatch( const IntVec2& position, const Map& mapToMutate )
{
    Vec2 positionCenter = static_cast<Vec2>(position) + Vec2( .5f, .5f );
    int intRadius = ceili( m_Radius );
    for ( int xRange = -intRadius; xRange <= intRadius; ++xRange )
    {
        for ( int yRange = -intRadius; yRange <= intRadius; ++yRange )
        {
            if ( xRange == 0 && yRange == 0 ) { continue; }

            IntVec2 offset = IntVec2( xRange, yRange );
            Vec2 offsetCenter = positionCenter + static_cast<Vec2>(offset) + Vec2( .5f, .5f );
            if ( Vec2::GetDistanceSquared( positionCenter, offsetCenter ) <= (m_Radius * m_Radius) )
            {
                if ( CheckPositionAgainst( position + offset, m_Neighbor, mapToMutate ) ) { return true; }
            }
        }
    }

    return false;
}

CAG_Circular::CAG_Circular( const XmlElement& element )
    : CellularAutomataGeneration( element )
{
    m_CaType = CellularAutomataType::CIRCULAR;

    m_Neighbor = &ParseXmlAttribute( element, "neighbor", *TileDefinition::DefaultDefinition );
}
