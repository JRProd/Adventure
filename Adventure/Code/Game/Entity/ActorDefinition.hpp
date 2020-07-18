#pragma once

#include "Game/Entity/EntityDefinition.hpp"

#include <map>

class SpriteAnimSet;

class ActorDefinition;
typedef std::map<const std::string, ActorDefinition*> ActorDefinitions;

class ActorDefinition: public EntityDefinition
{
public:
    static ActorDefinitions Definitions;

    static void PopulateFromXmlElement( const XmlElement& elementRoot );
    static ActorDefinition& GetFromName( const std::string& name );

    //-------------------------------------------------------------------------
    // Constructors
    ~ActorDefinition();
    ActorDefinition( const ActorDefinition& copy ) = default;

    //-------------------------------------------------------------------------
    // Actor Definition Accessors
    const SpriteAnimSet& GetSpriteAnimSet() const { return *m_AnimationSet; }

private:
    SpriteAnimSet* m_AnimationSet = nullptr;

    ActorDefinition( const XmlElement& element );
};