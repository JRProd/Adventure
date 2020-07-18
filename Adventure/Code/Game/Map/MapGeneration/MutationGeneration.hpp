#pragma once


#include "Engine/Core/Math/Primatives/IntVec2.hpp"
#include "Engine/Core/Math/Range/FloatRange.hpp"

#include "Game/Map/MapGeneration/MapGenerationStep.hpp"
#include "Game/Map/MapGeneration/SingleModificationStep.hpp"

class MutationGeneration: public MapGenerationStep
{
public:
    virtual ~MutationGeneration();
    MutationGeneration( const XmlElement& element );

    virtual void RunStepOnce( Map& mapToMutate, 
                              RandomNumberGenerator& rng ) override;

private:
    ModificationSteps m_MutationSteps;

    FloatRange m_MutationChance = FloatRange( .25f, .45f );
    int m_MaxMutations = 999'999'999;
};