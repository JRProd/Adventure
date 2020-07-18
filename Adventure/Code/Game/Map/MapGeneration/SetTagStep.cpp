#include "SetTagStep.hpp"

#include "Game/Map/Map.hpp"

#include "Engine/Console/Console.hpp"

SetTagStep::SetTagStep( const XmlElement& element )
{
    std::string commaTags = ParseXmlAttribute( element, "tags", "" );
    m_Tags = SplitStringOnDelimiter( commaTags, ',' );
}

bool SetTagStep::IndividualModification( IntVec2& mapPosition, Map& mapToMutate ) const
{
    Tags* tileTags = mapToMutate.GetTileTagsFromPosition( mapPosition );
    if ( tileTags == nullptr )
    {
        g_Console->Log( LOG_ERROR, Stringf( "SetTagStep: Invalid tile position (%s)", mapPosition.ToString().c_str() ) );
        return false;
    }

    tileTags->AddTags( m_Tags );
    return true;
}
