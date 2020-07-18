#include "Game/Map/MapGeneration/MapGenerationStep.hpp"

#include "Game/Map/MapGeneration/AllGeneration.hpp"
#include "Game/Map/MapGeneration/CellularAutomataGeneration.hpp"
#include "Game/Map/MapGeneration/FromImageGeneration.hpp"
#include "Game/Map/MapGeneration/IfTagStep.hpp"
#include "Game/Map/MapGeneration/MutationGeneration.hpp"
#include "Game/Map/MapGeneration/SetTagStep.hpp"
#include "Game/Map/MapGeneration/TileModificaitonStep.hpp"
#include "Game/Map/MapGeneration/WormGeneration.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/Math/RandomNumberGenerator.hpp"

MapGenerationStep* MapGenerationStep::CreateFromXml( const XmlElement& element )
{
    std::string generationStep = std::string( element.Name() );

    if ( generationStep == "All" ) { return new AllGeneration( element ); }
    if ( generationStep == "CellularAutomata" ) { return CellularAutomataGeneration::Create( element ); }
    if ( generationStep == "FromImage" ) { return new FromImageGeneration( element ); }
    if ( generationStep == "Mutate" ) { return new MutationGeneration( element ); }
    if ( generationStep == "Worm" ) { return WormGeneration::Create( element ); }

    g_Console->LogWTF( Stringf( "MapGenerationStep: ERROR Unknown generation step \"%s\"", generationStep.c_str() ) );
}

void MapGenerationStep::RunStep( Map& mapToMutate, RandomNumberGenerator& rng )
{
    if ( rng.Chance( m_ChanceToRun ) )
    {
        int iterations = m_Iterations.GetRandomInRange( rng );
        for ( int currentIter = 0; currentIter < iterations; ++currentIter )
        {
            RunStepOnce( mapToMutate, rng );
        }
    }
}

MapGenerationStep::MapGenerationStep( const XmlElement& element )
{
    m_ChanceToRun = ParseXmlAttribute( element, "chanceToRun", m_ChanceToRun );
    m_Iterations = ParseXmlAttribute( element, "iterations", m_Iterations );
}

SingleModificationStep* MapGenerationStep::CreateSingleModificationStep( const XmlElement& element )
{
    std::string modificationStepName = std::string( element.Name() );

    if ( modificationStepName == "IfTagStep" ) { return new IfTagStep( element ); }
    if ( modificationStepName == "SetTagStep" ) { return new SetTagStep( element ); }
    if ( modificationStepName == "TileStep" ) { return new TileModificaitonStep( element ); }

    g_Console->LogWTF( Stringf( "MutationGeneration: ERROR Unknown modification step \"%s\"", modificationStepName.c_str() ) );
}
