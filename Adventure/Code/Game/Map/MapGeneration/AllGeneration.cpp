#include "AllGeneration.hpp"

#include "Game/Map/Map.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"

AllGeneration::AllGeneration( const XmlElement& element )
    : MapGenerationStep( element )
{
    XmlElement const* modificationStep = element.FirstChildElement();
    if ( modificationStep == nullptr )
    {
        g_Console->LogWTF( Stringf( "AllGeneration: ERROR Must have at least one modification step" ) );
    }
    else
    {
        g_Console->Log( LOG_VERBOSE, Stringf( "AllGeneration: Adding modification step" ) );
    }

    while ( modificationStep )
    {
        m_ModificationSteps.push_back( CreateSingleModificationStep( *modificationStep ) );
        modificationStep = modificationStep->NextSiblingElement();
    }
}

void AllGeneration::RunStepOnce( Map& mapToMutate, RandomNumberGenerator& rng )
{
    UNUSED( rng );

    for ( int tileY = 0; tileY < mapToMutate.GetMapBounds().GetDimensions().y; ++tileY )
    {
        for ( int tileX = 0; tileX < mapToMutate.GetMapBounds().GetDimensions().x; ++tileX )
        {
            IntVec2 tilePosition = IntVec2( tileX, tileY );
            // For each of modification
            for ( SingleModificationStep* currentMutation : m_ModificationSteps )
            {
                // If that modification did not pass that step, go to next tile
                bool shouldContinue = currentMutation->IndividualModification( tilePosition,
                                                                               mapToMutate );
                if ( !shouldContinue ) { break; }
            }
        }
    }
}

