#include "Game/Map/MapGeneration/IfTagStep.hpp"

#include "Game/Map/Map.hpp"

#include "Engine/Console/Console.hpp"

IfTagStep::IfTagStep( const XmlElement& element )
{
    std::string commaTags = ParseXmlAttribute( element, "tags", "" );
    m_Tags.AddTags( SplitStringOnDelimiter( commaTags, ',' ) );
}

bool IfTagStep::IndividualModification( IntVec2& mapPosition, Map& mapToMutate ) const
{
    Tags* tileTags = mapToMutate.GetTileTagsFromPosition( mapPosition );
    if ( tileTags == nullptr )
    {
        g_Console->Log( LOG_ERROR, Stringf( "IfTagStep: Invalid tile position (%s)", mapPosition.ToString().c_str() ) );
        return false;
    }

    if ( tileTags->Contains( m_Tags ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

