#include "MutationGeneration.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/Math/RandomNumberGenerator.hpp"

#include "Game/Map/Map.hpp"
#include "Game/Map/MapGeneration/TileModificaitonStep.hpp"

MutationGeneration::~MutationGeneration()
{
    // For each of mutations
    for ( SingleModificationStep* currentMutation : m_MutationSteps )
    {
        delete currentMutation;
        currentMutation = nullptr;
    }
}

MutationGeneration::MutationGeneration( const XmlElement& element )
    : MapGenerationStep( element )
{
    XmlElement const* mutationStep = element.FirstChildElement();
    if ( mutationStep == nullptr )
    {
        g_Console->LogWTF( Stringf( "MutationGeneration: ERROR Must have at least one modification step" ) );
    }
    else
    {
        g_Console->Log( LOG_VERBOSE, Stringf( "MutationGeneration: Adding modification step" ) );
    }

    m_MutationChance = ParseXmlAttribute( element, "mutationChance", m_MutationChance );

    m_MaxMutations = ParseXmlAttribute( element, "maxMutations", m_MaxMutations );

    while ( mutationStep )
    {
        m_MutationSteps.push_back( CreateSingleModificationStep( *mutationStep ) );
        mutationStep = mutationStep->NextSiblingElement();
    }
}

void MutationGeneration::RunStepOnce( Map& mapToMutate,
                                      RandomNumberGenerator& rng )
{
    float perTileMutationChance = m_MutationChance.GetRandomInRange( rng );

    IntVec2 mapDimension = static_cast<IntVec2>(mapToMutate.GetMapBounds().GetDimensions());
    int mapSize = mapDimension.x * mapDimension.y;

    int currentSuccessfulMutations = 0;
    // Loop through all the tiles
    for ( int tileIndex = 0; tileIndex < mapSize; ++tileIndex )
    {
        // If the tile does not make the chance
        if ( !rng.Chance( perTileMutationChance ) ) { continue; }

        IntVec2 tilePosition = IntVec2( tileIndex % mapDimension.x,
                                        tileIndex / mapDimension.x );

        // For each of mutations
        for ( SingleModificationStep* currentMutation : m_MutationSteps )
        {
            // If that mutation did not pass that step, go to next tile
            bool shouldContinue = currentMutation->IndividualModification( tilePosition,
                                                                           mapToMutate );
            if ( !shouldContinue ) { break; }
        }

        currentSuccessfulMutations++;

        ////////////////////////////////////////////////////////////////////////////////
        // TEMP HACK
        // TODO: Fix max mutations to randomly select if more potential mutation spots 
        //  found
        ////////////////////////////////////////////////////////////////////////////////
        if ( currentSuccessfulMutations >= m_MaxMutations )
        {
            return;
        }
    }
}
