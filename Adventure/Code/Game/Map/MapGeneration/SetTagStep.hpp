#pragma once

#include "Game/Map/MapGeneration/SingleModificationStep.hpp"

#include "Engine/Core/Utils/StringUtils.hpp"

class SetTagStep: public SingleModificationStep
{
    friend class MapGenerationStep;
private:
    Strings m_Tags;

    SetTagStep( const XmlElement& element );

    bool IndividualModification( IntVec2& mapPosition, Map& mapToMutate ) const override;
};