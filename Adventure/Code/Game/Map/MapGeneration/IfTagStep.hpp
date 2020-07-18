#pragma once

#include "Game/Map/MapGeneration/SingleModificationStep.hpp"

#include "Engine/Core/Utils/StringUtils.hpp"
#include "Engine/Core/Tags.hpp"

class IfTagStep: public SingleModificationStep
{
    friend class MapGenerationStep;
private:
    Tags m_Tags;

    IfTagStep( const XmlElement& element );

    bool IndividualModification( IntVec2& mapPosition, Map& mapToMutate ) const override;
};