#pragma once

#include <vector>

#include "Engine/Core/Utils/XmlUtils.hpp"
#include "Engine/Core/Math/Range/IntRange.hpp"

class RandomNumberGenerator;

class Map;
class SingleModificationStep;

class MapGenerationStep;
typedef std::vector<MapGenerationStep*> MapGenerationSteps;

class MapGenerationStep
{
public:
    static MapGenerationStep* CreateFromXml( const XmlElement& element );

    virtual ~MapGenerationStep() = default;

    void RunStep(Map& mapToMutate, RandomNumberGenerator& rng);
    virtual void RunStepOnce( Map& mapToMutate, RandomNumberGenerator& rng ) = 0;

protected:
    float m_ChanceToRun = 1.f;
    IntRange m_Iterations = IntRange( 1 );

    MapGenerationStep( const XmlElement& element );

    SingleModificationStep* CreateSingleModificationStep( const XmlElement& element );
};