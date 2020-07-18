#pragma once

#include "Game/Map/MapGeneration/MapGenerationStep.hpp"
#include "Game/Map/MapGeneration/SingleModificationStep.hpp"

class TileDefinition;

enum class CellularAutomataType
{
    CARDINAL,
    MOORE,
    TAXICAB,
    CIRCULAR,
};
CellularAutomataType ParseXmlAttribute( const XmlElement& element,
                                        const char* attributeName,
                                        CellularAutomataType defaultType = CellularAutomataType::CARDINAL );

class CellularAutomataGeneration: public MapGenerationStep
{
public:
    static CellularAutomataGeneration* Create( const XmlElement& element );

    virtual ~CellularAutomataGeneration();

    void RunStepOnce( Map& mapToMutate, RandomNumberGenerator& rng ) override;

protected:
    CellularAutomataType m_CaType = CellularAutomataType::CARDINAL;
    float m_Radius = 1.f;

    CellularAutomataGeneration( const XmlElement& element );

    virtual bool PositionMatch( const IntVec2& position, const Map& mapToCheck ) = 0;

    bool CheckPositionAgainst( const IntVec2& tilePos, 
                               const TileDefinition* tileDefToCheck, 
                               const Map& mapToCheck ) const;

private:
    ModificationSteps m_ModificationSteps;
};