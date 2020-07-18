#include "WormGeneration.hpp"

#include "Game/Map/Map.hpp"
#include "Game/Map/MapGeneration/WG_Drunken.hpp"
#include "Game/Map/MapGeneration/WG_DrunkenDirection.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Math/RandomNumberGenerator.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"

WormType ParseXmlAttribute( const XmlElement& element, const char* attributeName, WormType defaultValue )
{
    const char* wormValue = element.Attribute( attributeName );
    if ( wormValue )
    {
        if ( Stricmp( wormValue, "Drunken" ) == 0 ) { return WormType::DRUNKEN; }
        if ( Stricmp( wormValue, "DrunkenDirection" ) == 0 ) { return WormType::DRUNKEN_DIRECTION; }
    }

    g_Console->Log( LOG_WARNING, Stringf( "WormGeneration::ParseXmlAttribute - Using WORM_TYPE default value \"%s\"",
                                          wormValue ) );
    return defaultValue;
}

WormGeneration::~WormGeneration()
{
    for ( SingleModificationStep* wormSteps : m_WormSteps )
    {
        delete wormSteps;
        wormSteps = nullptr;
    }
}

WormGeneration::WormGeneration( const XmlElement& element )
    : MapGenerationStep( element )
{
    m_WormType = ParseXmlAttribute( element, "wormType", m_WormType );
    m_WormNumber = ParseXmlAttribute( element, "wormNumber", m_WormNumber );
    m_WormLength = ParseXmlAttribute( element, "wormLength", m_WormLength );

    XmlElement const* wormStep = element.FirstChildElement();
    if ( wormStep == nullptr )
    {
        g_Console->LogWTF( Stringf( "WormGeneration: ERROR Must have at least one modification step" ) );
    }
    else
    {
        g_Console->Log( LOG_VERBOSE, Stringf( "WormGeneration: Adding modification step" ) );
    }

    while ( wormStep )
    {
        m_WormSteps.push_back( CreateSingleModificationStep( *wormStep ) );
        wormStep = wormStep->NextSiblingElement();
    }
}

WormGeneration* WormGeneration::Create( const XmlElement& element )
{
    WormType wormType = ParseXmlAttribute( element, "type", WormType::DRUNKEN );
    switch ( wormType )
    {
        case WormType::DRUNKEN:
            return new WG_Drunken( element );
        case WormType::DRUNKEN_DIRECTION:
            return new WG_DrunkenDirection( element );
        default:
            break;
    }

    std::string wormTypeString = ParseXmlAttribute( element, "type", "NO TYPE SPECIFIED" );
    g_Console->LogWTF( Stringf( " WormGeneration: ERROR \"%s\" is not a Worm Type", wormTypeString.c_str() ) );
}

void WormGeneration::RunStepOnce( Map& mapToMutate, RandomNumberGenerator& rng )
{
    // Get a random number of worms to run
    int numberOfWorms = m_WormNumber.GetRandomInRange( rng );

    // For each of the worms
    for ( int wormNumber = 0; wormNumber < numberOfWorms; ++wormNumber )
    {
        // Get the length of the current worm
        int maxWormLength = m_WormLength.GetRandomInRange( rng );

        // Find a starting position based on the sub-class
        IntVec2 wormPosition = ChooseStartPosition( mapToMutate, rng );
        for ( int wormLength = 0; wormLength < maxWormLength; ++wormLength )
        {
            // For each of modification
            for ( SingleModificationStep* currentMutation : m_WormSteps )
            {
                // If that v did not pass that step, go to next tile
                bool shouldContinue = currentMutation->IndividualModification( wormPosition,
                                                                               mapToMutate );
                if ( !shouldContinue ) { break; }
            }

            // Go to the next position based on the sub-class
            wormPosition = ChooseNextPosition( mapToMutate, rng );

//             Move the worm in a random position
//                         int direction = rng.IntLessThan( 4 );
//             
//                         IntVec2 oldPosition = wormPosition;
//                         switch ( direction )
//                         {
//                             case 0: // Right
//                                 wormPosition.x += 1;
//                                 break;
//                             case 1: // Up
//                                 wormPosition.y += 1;
//                                 break;
//                             case 2: // Left
//                                 wormPosition.x -= 1;
//                                 break;
//                             case 3: // Down
//                                 wormPosition.y -= 1;
//                                 break;
//                             default: // NO MOVE
//                     break;
//                         }
//             
//                         // If the new worm position is not valid, return to old position
//                         if ( !mapToMutate.IsValidTilePos( wormPosition ) )
//                         {
//                             wormPosition = oldPosition;
//                         }
        }
    }
}
