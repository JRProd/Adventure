#pragma once

#include <vector>

#include "Engine/Core/Utils/XmlUtils.hpp"

struct IntVec2;

class Map;

class SingleModificationStep;
typedef std::vector<SingleModificationStep*> ModificationSteps;

class SingleModificationStep
{
public:
    virtual bool IndividualModification( IntVec2& mapPosition,
                                         Map& mapToMutate ) const = 0;
};