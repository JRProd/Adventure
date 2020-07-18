#include "EntityDefinition.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/EngineCommon.hpp"

EntityDefinition::EntityDefinition( const XmlElement& element )
{
    m_EntityDefinitionName = ParseXmlAttribute( element, "name", m_EntityDefinitionName );
    if ( m_EntityDefinitionName == "NO_DEFINITION_NAME" )
    {
        g_Console->LogWTF( Stringf( "EntityDefinition: Entity must have \"name\" attribute" ) );
    }
    else
    {
        g_Console->Log( LOG_VERBOSE, Stringf( "EntityDefinition: Added entity \"%s\" to definition", m_EntityDefinitionName.c_str() ) );
    }

    ParseSizeElement( element.FirstChildElement( "Size" ) );

    ParseWalkElement( element.FirstChildElement( "Movement" ) );

    ParseHealthElement( element.FirstChildElement( "Health" ) );
}

void EntityDefinition::ParseSizeElement( const XmlElement* sizeElement )
{
    if ( sizeElement == nullptr )
    {
        return;
    }

    m_PhysicsRadius = ParseXmlAttribute( *sizeElement, "physicsSize", m_PhysicsRadius );
    m_VisualBounds = ParseXmlAttribute( *sizeElement, "visualSize", m_VisualBounds );
}

void EntityDefinition::ParseWalkElement( const XmlElement* walkElement )
{
    if ( walkElement == nullptr )
    {
        return;
    }

    m_CanWalk = ParseXmlAttribute( *walkElement, "canWalk", m_CanWalk );
    m_CanFly = ParseXmlAttribute( *walkElement, "canFly", m_CanFly );
    m_CanSwim = ParseXmlAttribute( *walkElement, "canSwim", m_CanSwim );
}

void EntityDefinition::ParseHealthElement( const XmlElement* healthElement )
{
    if ( healthElement == nullptr )
    {
        return;
    }

    m_MaxHealth = ParseXmlAttribute( *healthElement, "maxHealth", m_MaxHealth );
    m_StartingHealth = ParseXmlAttribute( *healthElement, "startingHealth", m_StartingHealth );
}
