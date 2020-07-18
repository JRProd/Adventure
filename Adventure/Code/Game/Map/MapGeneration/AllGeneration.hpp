#pragma once

#include "Game/Map/MapGeneration/MapGenerationStep.hpp"
#include "Game/Map/MapGeneration/SingleModificationStep.hpp"

class AllGeneration: public MapGenerationStep
{
public:
    AllGeneration( const XmlElement& element );

    void RunStepOnce( Map& mapToMutate, RandomNumberGenerator& rng ) override;

private:
    ModificationSteps m_ModificationSteps;
};