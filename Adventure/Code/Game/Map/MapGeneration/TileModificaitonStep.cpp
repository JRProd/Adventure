#include "TileModificaitonStep.hpp"

#include "Engine/Console/Console.hpp"

#include "Game/Map/Map.hpp"
#include "Game/Map/Tile/Tile.hpp"
#include "Game/Map/Tile/TileDefinition.hpp"

bool TileModificaitonStep::IndividualModification( IntVec2& mapPosition, Map& mapToMutate ) const
{
    Tile& currentTile = *mapToMutate.GetTileFromPosition( mapPosition );

    if ( m_TileFilter == nullptr )
    {
        currentTile.SetTileDefinition( m_TileMutation );
        return true;
    }
    // Only filter if the tile filter exists
    else if ( currentTile.GetTileDefinition() == *m_TileFilter )
    {
        currentTile.SetTileDefinition( m_TileMutation );
        return true;
    }
    else
    {
        return false;
    }
}

TileModificaitonStep::TileModificaitonStep( const XmlElement& element )
{
    std::string tileMutationName = ParseXmlAttribute( element, 
                                                      "tileModification", 
                                                      "NO_DEFINITION_NAME" );
    if ( tileMutationName == "NO_DEFINITION_NAME" )
    {
        g_Console->LogWTF( Stringf( "TileModificaitonStep: ERROR Tile modification must have a valid \"tileModification\" attribute" ) );
    }

    m_TileMutation = &TileDefinition::GetFromName( tileMutationName );
    if ( m_TileMutation == TileDefinition::DefaultDefinition )
    {
        g_Console->Log( LOG_ERROR, Stringf( "TileModificaitonStep: ERROR Tile \"%s\" does not exist", tileMutationName.c_str() ) );
    }

    std::string tileFilterName = ParseXmlAttribute( element,
                                                      "tileFilter",
                                                      "NO_DEFINITION_NAME" );
    m_TileFilter = &TileDefinition::GetFromName( tileFilterName );
    if ( m_TileFilter == TileDefinition::DefaultDefinition )
    {
        m_TileFilter = nullptr;
    }
}

