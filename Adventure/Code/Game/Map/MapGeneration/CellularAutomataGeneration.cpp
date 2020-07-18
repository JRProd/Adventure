#include "Game/Map/MapGeneration/CellularAutomataGeneration.hpp"

#include "Game/Map/Map.hpp"
#include "Game/Map/MapGeneration/CAG_Cardinal.hpp"
#include "Game/Map/MapGeneration/CAG_Circular.hpp"
#include "Game/Map/MapGeneration/CAG_Moore.hpp"
#include "Game/Map/MapGeneration/CAG_Taxicab.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"
#include "Engine/Core/Math/Primatives/IntVec2.hpp"

CellularAutomataType ParseXmlAttribute( const XmlElement& element, const char* attributeName, CellularAutomataType defaultType )
{
    const char* caType = element.Attribute( attributeName );
    if ( caType )
    {
        if ( Stricmp( caType, "Cardinal" ) == 0 ) return CellularAutomataType::CARDINAL;
        if ( Stricmp( caType, "Circular" ) == 0 ) return CellularAutomataType::CIRCULAR;
        if ( Stricmp( caType, "Moore" ) == 0 ) return CellularAutomataType::MOORE;
        if ( Stricmp( caType, "Taxicab" ) == 0 ) return CellularAutomataType::TAXICAB;
    }

    return defaultType;
}

STATIC CellularAutomataGeneration* CellularAutomataGeneration::Create( const XmlElement& element )
{
    CellularAutomataType caType = ParseXmlAttribute( element, "type", CellularAutomataType::CARDINAL );
    switch ( caType )
    {
        case CellularAutomataType::CARDINAL:
            return new CAG_Cardinal( element );
        case CellularAutomataType::MOORE:
            return new CAG_Moore( element );
        case CellularAutomataType::TAXICAB:
            return new CAG_Taxicab( element );
        case CellularAutomataType::CIRCULAR:
            return new CAG_Circular( element );
        default:
            break;
    }

    std::string caTypeString = ParseXmlAttribute( element, "type", "NO TYPE SPECIFIED" );
    g_Console->LogWTF( Stringf( " CellularAutomataGeneration: ERROR \"%s\" is not a Cellular Automata Type", caTypeString.c_str() ) );
}

CellularAutomataGeneration::~CellularAutomataGeneration()
{
    for ( SingleModificationStep* step : m_ModificationSteps )
    {
        delete step;
        step = nullptr;
    }
}

void CellularAutomataGeneration::RunStepOnce( Map& mapToMutate, RandomNumberGenerator& rng )
{
    UNUSED( rng );

    int mapVectorSize = mapToMutate.GetMapBounds().GetDimensions().x * mapToMutate.GetMapBounds().GetDimensions().y;
    std::vector<bool> doesTileGetModified;
    doesTileGetModified.reserve( mapVectorSize );

    // Check to see what tiles positions would be changed
    for ( int tileY = 0; tileY < mapToMutate.GetMapBounds().GetDimensions().y; ++tileY )
    {
        for ( int tileX = 0; tileX < mapToMutate.GetMapBounds().GetDimensions().x; ++tileX )
        {
            IntVec2 tilePosition = IntVec2( tileX, tileY );
            doesTileGetModified.push_back( PositionMatch( tilePosition, mapToMutate ) );
        }
    }

    // Change those tile positions
    int mapSizeX = mapToMutate.GetMapBounds().GetDimensions().x;
    for ( int tileY = 0; tileY < mapToMutate.GetMapBounds().GetDimensions().y; ++tileY )
    {
        for ( int tileX = 0; tileX < mapToMutate.GetMapBounds().GetDimensions().x; ++tileX )
        {
            IntVec2 tilePosition = IntVec2( tileX, tileY );
            int vectorIndex = tileY * mapSizeX + tileX;

            // If the tile position should mutate
            if ( doesTileGetModified.at( vectorIndex ) )
            {
                for ( SingleModificationStep* singleModificaiton : m_ModificationSteps )
                {
                    bool shouldContinue = singleModificaiton->IndividualModification( tilePosition, mapToMutate );
                    if ( !shouldContinue ) { break; }
                }
            }
        }
    }
}

CellularAutomataGeneration::CellularAutomataGeneration( const XmlElement& element )
    : MapGenerationStep(element)
{
    m_Radius = ParseXmlAttribute( element, "radius", m_Radius );

    XmlElement const* caStep = element.FirstChildElement();
    if ( caStep == nullptr )
    {
        g_Console->LogWTF( Stringf( "WormGeneration: ERROR Must have at least one modification step" ) );
    }
    else
    {
        g_Console->Log( LOG_VERBOSE, Stringf( "WormGeneration: Adding modification step" ) );
    }

    while ( caStep )
    {
        m_ModificationSteps.push_back( CreateSingleModificationStep( *caStep ) );
        caStep = caStep->NextSiblingElement();
    }
}

bool CellularAutomataGeneration::CheckPositionAgainst( const IntVec2& tilePos, const TileDefinition* tileDefToCheck, const Map& mapToCheck ) const
{ 
    if ( mapToCheck.IsValidTilePos( tilePos ) )
    {
        const Tile* tileCheck = mapToCheck.GetTileFromPosition( tilePos );
        if ( tileDefToCheck && *tileDefToCheck == tileCheck->GetTileDefinition() ) { return true; }
    }

    return false;
}
