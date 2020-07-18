#pragma once

#include "Game/Map/MapGeneration/SingleModificationStep.hpp"

class TileDefinition;

class TileModificaitonStep: public SingleModificationStep
{
    friend class MapGenerationStep;
private:
    TileDefinition* m_TileMutation = nullptr;
    TileDefinition* m_TileFilter = nullptr;

    TileModificaitonStep( const XmlElement& element );

    virtual bool IndividualModification( IntVec2& mapPosition,
                                     Map& mapToMutate ) const override;
};