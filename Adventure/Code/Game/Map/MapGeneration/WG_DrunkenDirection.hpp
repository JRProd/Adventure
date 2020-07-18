#pragma once

#include "Game/Map/MapGeneration/WormGeneration.hpp"

class WG_DrunkenDirection: public WormGeneration
{
    friend class WormGeneration;
protected:
    const IntVec2 ChooseStartPosition( Map& mapToMutate, RandomNumberGenerator& rng ) override;
    const IntVec2 ChooseNextPosition( Map& mapToMutate, RandomNumberGenerator& rng ) override;

private:
    IntVec2 m_CurrentPosition = IntVec2::ZERO;
    float m_DirectionChance[ 4 ] = { .25f, .25f, .25f, .25f }; // EAST, NORTH, WEST, SOUTH

    WG_DrunkenDirection( const XmlElement& element );
};