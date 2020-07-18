#pragma once

#include "Game/Map/MapGeneration/MapGenerationStep.hpp"
#include "Game/Map/MapGeneration/SingleModificationStep.hpp"

#include "Engine/Core/Math/Primatives/IntVec2.hpp"

enum class WormType
{
    DRUNKEN,
    DRUNKEN_DIRECTION,
};
WormType ParseXmlAttribute( const XmlElement& element,
                            const char* attributeName,
                            WormType defaultType );

class WormGeneration: public MapGenerationStep
{
public:
    static WormGeneration* Create( const XmlElement& element );

    virtual ~WormGeneration();

    virtual void RunStepOnce( Map& mapToMutate, RandomNumberGenerator& rng ) override;

protected:
    WormType m_WormType = WormType::DRUNKEN;
    IntRange m_WormNumber = IntRange( 4, 7 );
    IntRange m_WormLength = IntRange( 5, 10 );

    WormGeneration( const XmlElement& element );

    virtual const IntVec2 ChooseStartPosition( Map& mapToMutate, RandomNumberGenerator& rng ) = 0;
    virtual const IntVec2 ChooseNextPosition( Map& mapToMutate, RandomNumberGenerator& rng ) = 0;

private:
    ModificationSteps m_WormSteps;
};