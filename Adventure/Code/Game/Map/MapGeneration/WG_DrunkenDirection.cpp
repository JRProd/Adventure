#include "Game/Map/MapGeneration/WG_DrunkenDirection.hpp"

#include "Game/Map/Map.hpp"

#include "Engine/Core/Math/RandomNumberGenerator.hpp"

const IntVec2 WG_DrunkenDirection::ChooseStartPosition( Map& mapToMutate, RandomNumberGenerator& rng )
{
    m_CurrentPosition = IntVec2( rng.IntInRange( static_cast<int>(mapToMutate.GetMapBounds().mins.x),
                                                 static_cast<int>(mapToMutate.GetMapBounds().maxs.x - 1) ),
                                 rng.IntInRange( static_cast<int>(mapToMutate.GetMapBounds().mins.y),
                                                 static_cast<int>(mapToMutate.GetMapBounds().maxs.y - 1) ) );
    return m_CurrentPosition;
}

const IntVec2 WG_DrunkenDirection::ChooseNextPosition( Map& mapToMutate, RandomNumberGenerator& rng )
{
    // Move the worm in a random position

    IntVec2 oldPosition = m_CurrentPosition;
    if ( rng.Chance( m_DirectionChance[ 0 ] ) )
    {
        m_CurrentPosition.x += 1;
    }
    else if ( rng.Chance( m_DirectionChance[ 1 ] ) )
    {
        m_CurrentPosition.y += 1;
    }
    else if ( rng.Chance( m_DirectionChance[ 2 ] ) )
    {
        m_CurrentPosition.x -= 1;
    }
    else if ( rng.Chance( m_DirectionChance[ 3 ] ) )
    {
        m_CurrentPosition.y -= 1;
    }

    // If the new worm position is not valid, return to old position
    if ( !mapToMutate.IsValidTilePos( m_CurrentPosition ) )
    {
        m_CurrentPosition = oldPosition;
    }

    return m_CurrentPosition;
}

WG_DrunkenDirection::WG_DrunkenDirection( const XmlElement& element )
    : WormGeneration( element )
{
    // Direction to favor
    float direction = ParseXmlAttribute( element, "direction", 0.f );

    // Intensity of the favored direction
    float intensity = ParseXmlAttribute( element, "intensity", .5f );
    // Scale 0~1 to 0~.25 
    intensity = ClampZeroToOne( intensity );
    intensity *= .25f;

    float c = cosfDegrees( direction ) * intensity;
    float s = sinfDegrees( direction ) * intensity;

    // Modify the direction chance by the direction
    m_DirectionChance[ 0 ] += c;
    m_DirectionChance[ 1 ] += s;
    m_DirectionChance[ 2 ] -= c;
    m_DirectionChance[ 3 ] -= s;
}

