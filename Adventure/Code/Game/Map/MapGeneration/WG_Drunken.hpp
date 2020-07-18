#pragma once

#include "Game/Map/MapGeneration/WormGeneration.hpp"

class WG_Drunken: public WormGeneration
{
    friend class WormGeneration;

protected:
    const IntVec2 ChooseStartPosition( Map& mapToMutate, RandomNumberGenerator& rng ) override;
    const IntVec2 ChooseNextPosition( Map& mapToMutate, RandomNumberGenerator& rng ) override;

private:
    IntVec2 m_CurrentPosition = IntVec2::ZERO;

    WG_Drunken( const XmlElement& element );
};