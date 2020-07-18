#include "Game/Map/MapGeneration/WG_Drunken.hpp"

#include "Game/Map/Map.hpp"

#include "Engine/Core/Math/RandomNumberGenerator.hpp"

const IntVec2 WG_Drunken::ChooseStartPosition( Map& mapToMutate, RandomNumberGenerator& rng )
{
    m_CurrentPosition = IntVec2( rng.IntInRange( static_cast<int>(mapToMutate.GetMapBounds().mins.x),
                                                 static_cast<int>(mapToMutate.GetMapBounds().maxs.x - 1) ),
                                 rng.IntInRange( static_cast<int>(mapToMutate.GetMapBounds().mins.y),
                                                 static_cast<int>(mapToMutate.GetMapBounds().maxs.y - 1) ) );
    return m_CurrentPosition;
}

const IntVec2 WG_Drunken::ChooseNextPosition( Map& mapToMutate, RandomNumberGenerator& rng )
{
    // Move the worm in a random position
    int direction = rng.IntLessThan( 4 );

    IntVec2 oldPosition = m_CurrentPosition;
    switch ( direction )
    {
        case 0: // Right
            m_CurrentPosition.x += 1;
            break;
        case 1: // Up
            m_CurrentPosition.y += 1;
            break;
        case 2: // Left
            m_CurrentPosition.x -= 1;
            break;
        case 3: // Down
            m_CurrentPosition.y -= 1;
            break;
        default: // NO MOVE
            break;
    }

    // If the new worm position is not valid, return to old position
    if ( !mapToMutate.IsValidTilePos( m_CurrentPosition ) )
    {
        m_CurrentPosition = oldPosition;
    }

    return m_CurrentPosition;
}

WG_Drunken::WG_Drunken( const XmlElement& element )
    : WormGeneration( element )
{
}

