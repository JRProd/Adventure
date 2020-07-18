#include "MapDefinition.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/EngineCommon.hpp"

#include "Game/Map/MapGeneration/MutationGeneration.hpp"

STATIC MapDefinitions MapDefinition::Definitions;

STATIC void MapDefinition::PopulateFromXmlElement( const XmlElement& elementRoot )
{
    XmlElement const* element = elementRoot.FirstChildElement();
    while ( element )
    {
        MapDefinition* madeDefinition = new MapDefinition( *element );
        Definitions[ madeDefinition->m_MapDefinitionName ] = madeDefinition;
        element = element->NextSiblingElement();
    }
}

STATIC MapDefinition& MapDefinition::GetFromName( const std::string& name )
{
    MapDefinitions::const_iterator findMapDef = Definitions.find( name );
    if ( findMapDef != Definitions.cend() )
    {
        return *findMapDef->second;
    }

    g_Console->LogWTF( Stringf( "MapDefinition: Could not find \"%s\" map definition",
                                name.c_str() ) );
}

void MapDefinition::RunGenerationSteps( Map& defaultFilledMap, RandomNumberGenerator& rng ) const
{
    for ( MapGenerationStep* generationStep : m_GenerationSteps )
    {
        generationStep->RunStep( defaultFilledMap, rng );
    }
}

MapDefinition::MapDefinition( const XmlElement& element )
{
    m_MapDefinitionName = ParseXmlAttribute( element, "name", m_MapDefinitionName );
    if ( m_MapDefinitionName == "NO_DEFINITION_NAME" )
    {
        g_Console->LogWTF( "MapDefinition: Map must have \"name\" attribute" );
    }
    else
    {
        g_Console->Log( LOG_VERBOSE, Stringf( "MapDefinition: Adding map \"%s\" to definitions",
                                              m_MapDefinitionName.c_str() ) );
    }

    m_MapSize = ParseXmlAttribute( element, "mapSize", IntVec2::ZERO );

    XmlElement const* startingTiles = element.FirstChildElement( "StartingTiles" );
    if ( startingTiles == nullptr )
    {
        g_Console->LogWTF( "MapDefinition: Map must have \"StartingTiles\" child element" );
    }

    ParseMapStartingTiles( *startingTiles );

    ParseMapGenerationSteps( element.FirstChildElement( "Generation" ) );
}

void MapDefinition::ParseMapStartingTiles( const XmlElement& startingTileElement )
{
    m_DefaultFillTile = &TileDefinition::GetFromName( ParseXmlAttribute( startingTileElement, 
                                                                             "fillTile" ) );
    if ( m_DefaultFillTile == TileDefinition::DefaultDefinition )
    {
        g_Console->LogWTF( "MapDefinition: StartingTiles element must have \"fillTile\" attribute" );
    }


    m_BoundaryTile = &TileDefinition::GetFromName( ParseXmlAttribute( startingTileElement, 
                                                                      "boundaryTile" ) );
    if ( m_BoundaryTile == TileDefinition::DefaultDefinition )
    {
        g_Console->LogWTF( "MapDefinition: StartingTiles element must have \"boundaryTile\" attribute" );
    }


    m_InaccesableTile = &TileDefinition::GetFromName( ParseXmlAttribute( startingTileElement, 
                                                                         "inaccesableTile" ) );
}

void MapDefinition::ParseMapGenerationSteps( const XmlElement* generationElement )
{
    if ( generationElement == nullptr )
    {
        g_Console->Log( LOG_LOG, Stringf( "MapDefinition: Map has no generation steps" ) );
        return;
    }
    else
    {
        g_Console->Log( LOG_VERBOSE, Stringf( "MapDefinition: Map generation steps" ) );
    }

    const XmlElement* generationStep = generationElement->FirstChildElement();
    std::string test = generationStep->Name();
    if ( generationStep == nullptr )
    {
        g_Console->LogWTF( Stringf( "MapDefinition: ERROR Generation found with ZERO steps" ) );
    }

    while ( generationStep )
    {
        m_GenerationSteps.push_back( MapGenerationStep::CreateFromXml( *generationStep ) );
        generationStep = generationStep->NextSiblingElement();
    }
}
