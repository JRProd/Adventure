#include "Game/Map/MapGeneration/CAG_Cardinal.hpp"

#include "Game/Map/Tile/TileDefinition.hpp"

#include "Engine/Core/Math/Primatives/IntVec2.hpp"

bool CAG_Cardinal::PositionMatch( const IntVec2& position, const Map& mapToMutate )
{
    int intRadius = roundi( m_Radius );
    for ( int range = 0; range < intRadius; ++range )
    {
        // Get the nth east tile
        IntVec2 east = IntVec2( 1 + range, 0 );
        if ( CheckPositionAgainst( position + east, m_EastTile, mapToMutate ) ) { return true; }

        IntVec2 north = IntVec2( 0, 1 + range );
        if ( CheckPositionAgainst( position + north, m_NorthTile, mapToMutate ) ) { return true; }

        IntVec2 west = IntVec2( -1 - range, 0 );
        if ( CheckPositionAgainst( position + west, m_WestTile, mapToMutate ) ) { return true; }

        IntVec2 sount = IntVec2( 0, -1 - range );
        if ( CheckPositionAgainst( position + sount, m_SouthTile, mapToMutate ) ) { return true; }
    }

    return false;
}

CAG_Cardinal::CAG_Cardinal( const XmlElement& element )
    : CellularAutomataGeneration( element )
{
    m_CaType = CellularAutomataType::CARDINAL;

    // If the direction isn't specified, check all directions
    const char* tileName = element.Attribute( "neighbor" );
    if ( tileName )
    {
        m_EastTile = &TileDefinition::GetFromName( tileName );
        m_NorthTile = m_EastTile;
        m_WestTile = m_EastTile;
        m_SouthTile = m_EastTile;

    }
    else
    {
        tileName = element.Attribute( "eastTile" );
        if ( tileName ) { m_EastTile = &TileDefinition::GetFromName( tileName ); }
        tileName = element.Attribute( "northTile" );
        if ( tileName ) { m_NorthTile = &TileDefinition::GetFromName( tileName ); }
        tileName = element.Attribute( "westTile" );
        if ( tileName ) { m_WestTile = &TileDefinition::GetFromName( tileName ); }
        tileName = element.Attribute( "southTile" );
        if ( tileName ) { m_SouthTile = &TileDefinition::GetFromName( tileName ); }
    }
}
