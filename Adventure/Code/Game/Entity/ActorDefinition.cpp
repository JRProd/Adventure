#include "ActorDefinition.hpp"

#include "Game/GlobalSystems.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/Sprite/SpriteAnimSet.hpp"

STATIC ActorDefinitions ActorDefinition::Definitions;

void ActorDefinition::PopulateFromXmlElement( const XmlElement& elementRoot )
{
    XmlElement const* element = elementRoot.FirstChildElement();
    while ( element )
    {
        ActorDefinition* madeDefinition = new ActorDefinition( *element );
        Definitions[ madeDefinition->m_EntityDefinitionName ] = madeDefinition;
        element = element->NextSiblingElement();
    }
}

ActorDefinition& ActorDefinition::GetFromName( const std::string& name )
{
    ActorDefinitions::const_iterator findActorDef = Definitions.find( name );
    if ( findActorDef != Definitions.cend() )
    {
        return *findActorDef->second;
    }

    g_Console->LogWTF( Stringf( "MapDefinition: Could not find \"%s\" map definition",
                                name.c_str() ) );
}

ActorDefinition::~ActorDefinition()
{
    delete m_AnimationSet;
    m_AnimationSet = nullptr;
}

static SpriteAnimDefinition* CreateSpriteAnimDefinition( const SpriteSheet& sheet, const XmlElement& animationElement,
    const float defaultFps )
{
    const std::string spriteIndexesString = ParseXmlAttribute( animationElement, "spriteIndexes", "" );
    const float fps = ParseXmlAttribute( animationElement, "fps", defaultFps );
    if( spriteIndexesString.empty() )
    {
#if !defined(ENGINE_DISABLE_CONSOLE)
        g_Console->LogWTF( Stringf( "SpriteAnimSet: ERROR Animations must have a \"spriteIndexes\" attribute" ) );
#endif // !defined(ENGINE_DISABLE_CONSOLE)
    }
    Strings spriteIndexes = SplitStringOnDelimiter( spriteIndexesString, ',' );

    std::vector<int> intIndexes;
    for( std::string& spriteIndex : spriteIndexes )
    {
        intIndexes.push_back( stoi( spriteIndex ) );
    }

    const float duration = static_cast<float>( intIndexes.size() ) / fps;

    return new SpriteAnimDefinition( sheet, intIndexes, duration );
}

static SpriteAnimSet* CreateAnimationSet( RenderContext* renderer, const XmlElement& element )
{
    std::string spriteAnimSheetFile = ParseXmlAttribute( element, "spriteSheet", "" );
    IntVec2 spriteAnimSheetSize = ParseXmlAttribute( element, "spriteLayout", IntVec2( 1, 1 ) );
    float defaultFps = ParseXmlAttribute( element, "fps", 10.f );
    if( spriteAnimSheetFile.empty() )
    {
#if !defined(ENGINE_DISABLE_CONSOLE)
        g_Console->LogWTF( Stringf( "SpriteAinmSet: ERROR Sprite Animation Set must have a \"spriteSheet\" attribute" )
        );
#endif // !defined(ENGINE_DISABLE_CONSOLE)
    }

    SpriteSheet* spriteSheet = renderer->CreateOrGetSpriteSheetFromFile( spriteAnimSheetFile, spriteAnimSheetSize );

    std::map<const std::string, SpriteAnimDefinition*> animationDefinitions;
    const XmlElement* animation = element.FirstChildElement();
    while( animation )
    {
        std::string animName = animation->Name();
        std::map<const std::string, SpriteAnimDefinition*>::const_iterator found = animationDefinitions.
            find( animName );
        if( found != animationDefinitions.cend() )
        {
#if !defined(ENGINE_DISABLE_CONSOLE)
            g_Console->Log( LOG_WARNING,
                Stringf( "SpriteAnimSet: Overriding previous animation \"%s\"", animName.c_str() )
            );
#endif // !defined(ENGINE_DISABLE_CONSOLE)
        }

        animationDefinitions[ animName ] = CreateSpriteAnimDefinition( *spriteSheet, *animation, defaultFps );
        animation = animation->NextSiblingElement();
    }

    return new SpriteAnimSet( animationDefinitions );
}

ActorDefinition::ActorDefinition( const XmlElement& element )
    : EntityDefinition( element )
{
    const XmlElement* animaitonElement = element.FirstChildElement( "Animations" );
    if ( animaitonElement )
    {
        m_AnimationSet = CreateAnimationSet( g_Renderer, *animaitonElement );
    }
}
