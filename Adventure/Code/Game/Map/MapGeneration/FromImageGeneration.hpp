#pragma once
                                                                                                   
#include "Engine/Core/Image.hpp"
#include "Engine/Core/Math/Range/FloatRange.hpp"

#include "Game/Map/MapGeneration/MapGenerationStep.hpp"

class FromImageGeneration: public MapGenerationStep
{
public:
    virtual ~FromImageGeneration();
    FromImageGeneration( const XmlElement& element );

    void RunStepOnce( Map& mapToMutate, RandomNumberGenerator& rng ) override;

private:
    Image m_ImageForGeneration;
    IntRange m_Rotations = IntRange(0, 3);
    FloatRange m_AlignmentU = FloatRange(0.f, 1.f);
    FloatRange m_AlignmentV = FloatRange(0.f,1.f);
    float m_ChanceToMirror = 0.f;
    float m_ChancePerTile = 1.f;
};