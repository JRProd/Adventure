#include "TileDefinition.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/Utils/XmlUtils.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/Sprite/SpriteSheet.hpp"

#include "Game/GlobalSystems.hpp"

TileDefinition& ParseXmlAttribute( const XmlElement& element,
                                    const char* attributeName,
                                    TileDefinition& defaultValue )
{
    const char* tileDefName = element.Attribute( attributeName );
    if ( tileDefName )
    {
        return TileDefinition::GetFromName( tileDefName, defaultValue );
    }

    g_Console->Log( LOG_WARNING, Stringf( "TileDefinition::ParseXmlAttributre - No attributre \"%s\", using \"%s\" tile defintion",
                                          attributeName, defaultValue.GetTileDefName().c_str() ) );
    return defaultValue;
}

STATIC TileDefinitions TileDefinition::Definitions;

STATIC void TileDefinition::PopulateFromXmlElement( const XmlElement& elementRoot )
{
    // Finds and loads the correct sprite sheet
    std::string tileSpriteSheet = elementRoot.Attribute( "spriteSheet" );
    IntVec2 tileSpriteLayout = ParseXmlAttribute( elementRoot, "layout", IntVec2( 1, 1 ) );

    s_TileSpriteSheet = g_Renderer->CreateOrGetSpriteSheetFromFile( tileSpriteSheet,
                                                                    tileSpriteLayout );

    DefaultDefinition = new TileDefinition( "DEFAULT_DEFINITION" );

    // Begins to create elements based on individual tile definitions
    XmlElement const* element = elementRoot.FirstChildElement();
    while ( element )
    {
        TileDefinition* madeDefinition = new TileDefinition( *element );
        Definitions[ madeDefinition->m_TileDefinitionName ] = madeDefinition;

        element = element->NextSiblingElement();
    }
}

STATIC TileDefinition& TileDefinition::GetFromName( const std::string& name,
                                                    TileDefinition& defaultDefinition )
{
    TileDefinitions::const_iterator findTileDef = Definitions.find( name );
    if ( findTileDef != Definitions.cend() )
    {
        return *findTileDef->second;
    }

    g_Console->Log( LOG_WARNING, Stringf( "TileDefinition: Could not find \"%s\" tile definition, using \"%s\" tile definition instead",
                                          name.c_str(), defaultDefinition.m_TileDefinitionName.c_str() ) );
    return defaultDefinition;
}

TileDefinition& TileDefinition::GetFromColor( const Rgba8& color, TileDefinition& defaultDefinition )
{
    TileDefinitions::const_iterator currentTile = Definitions.cbegin();
    for ( ; currentTile != Definitions.cend(); ++currentTile )
    {
        if ( Rgba8::CompareRGB( currentTile->second->GetTileDefMapColor(), color ) )
        {
            return *currentTile->second;
        }
    }

    g_Console->Log( LOG_ERROR, Stringf( "TileDefinition: Could not find tile with color %s, using \"%s\" tile definition instead",
                                        ConvertToString( color ).c_str(), defaultDefinition.m_TileDefinitionName.c_str() ) );
    return defaultDefinition;
}

const SpriteDefinition& TileDefinition::GetTileDefSpriteDefinition() const
{
    return s_TileSpriteSheet->GetSpriteDefinition( m_TileSpriteIndex );
}

bool TileDefinition::operator==( const TileDefinition& rhs ) const
{
    return m_TileDefinitionName == rhs.m_TileDefinitionName;
}

bool TileDefinition::operator!=( const TileDefinition& rhs ) const
{
    return !(operator==( rhs ));
}

TileDefinition::TileDefinition( const XmlElement& element )
{

    m_TileDefinitionName = ParseXmlAttribute( element, "name", m_TileDefinitionName );
    if ( m_TileDefinitionName == "NO_DEFINITION_NAME" )
    {
        g_Console->LogWTF( "TileDefinition: Tile must have \"name\" attribute" );
    }
    else
    {
        g_Console->Log( LOG_VERBOSE, Stringf( "TileDefinition: Adding tile \"%s\" to definitions",
                                              m_TileDefinitionName.c_str() ) );
    }

    IntVec2 spriteCoords = ParseXmlAttribute( element, "spriteCoords", IntVec2::ZERO );
    m_TileSpriteIndex = s_TileSpriteSheet->GetSpriteSheetSize().x * spriteCoords.y + spriteCoords.x;
    Vec2 uvMins = Vec2::ZERO;
    Vec2 uvMaxs = Vec2::ONE;
    s_TileSpriteSheet->GetSpriteUVs( m_TileSpriteIndex, OUT_PARAM uvMins, OUT_PARAM uvMaxs );
    m_TileSpriteUVs.mins = uvMins;
    m_TileSpriteUVs.maxs = uvMaxs;

    m_TileMapColor = ParseXmlAttribute( element, "mapColor", m_TileMapColor );
    if ( m_TileMapColor == Rgba8::CLEAR )
    {
        g_Console->LogWTF( "TileDefintion: Tile must have \"mapColor\" attribute" );
    }
    m_TileTint = ParseXmlAttribute( element, "spriteTint", m_TileTint );

    m_AllowWalk = ParseXmlAttribute( element, "allowWalk", m_AllowWalk );
    m_AllowFly = ParseXmlAttribute( element, "allowFly", m_AllowFly );
    m_AllowSwim = ParseXmlAttribute( element, "allowSwim", m_AllowSwim );
    m_AllowSight = ParseXmlAttribute( element, "allowSight", m_AllowSight );
    m_AllowProjectile = ParseXmlAttribute( element, "allowProjectiles", m_AllowProjectile );
}

TileDefinition::TileDefinition( const std::string& name )
{
    m_TileDefinitionName = name;
}

STATIC SpriteSheet* TileDefinition::s_TileSpriteSheet = nullptr;
STATIC TileDefinition* TileDefinition::DefaultDefinition = nullptr;
