#pragma once

#include <map>

#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Math/Primatives/AABB2.hpp"
#include "Engine/Core/Utils/XmlUtils.hpp"
#include "Engine/Renderer/Sprite/SpriteDefinition.hpp"

class TileDefinition;
typedef std::map<const std::string, TileDefinition*> TileDefinitions;

TileDefinition& ParseXmlAttribute( const XmlElement& element,
                                    const char* attributeName,
                                    TileDefinition& defaultValue );

class TileDefinition
{
public:
    static TileDefinitions Definitions;
    static TileDefinition* DefaultDefinition;

    //-------------------------------------------------------------------------
    // Creates tile definitions from a xml object
    static void PopulateFromXmlElement( const XmlElement& elementRoot );
    static TileDefinition& GetFromName( const std::string& name,
                                        TileDefinition& defaultDefinition = *DefaultDefinition );
    static TileDefinition& GetFromColor( const Rgba8& color,
                                               TileDefinition& defaultDefinition = *DefaultDefinition );

    //-------------------------------------------------------------------------
    // Constructors
    TileDefinition( const TileDefinition& copy ) = default;

    //-------------------------------------------------------------------------
    // Tile Definitions Accessors
    const SpriteSheet& GetTileDefSpriteSheet() const { return *s_TileSpriteSheet; }
    const std::string GetTileDefName() const { return m_TileDefinitionName; }
    const Rgba8 GetTileDefMapColor() const { return m_TileMapColor; }
    const Rgba8 GetTileDefTint() const { return m_TileTint; }
    const AABB2 GetTileDefSpriteUVs() const { return m_TileSpriteUVs; }

    const SpriteDefinition& GetTileDefSpriteDefinition() const;

    bool DoesTileDefAllowWalk() const { return m_AllowWalk; }
    bool DoesTileDefAllowFly() const { return m_AllowFly; }
    bool DoesTileDefAllowSwim() const { return m_AllowSwim; }
    bool DoesTileDefAllowSight() const { return m_AllowSight; }
    bool DoesTileDefAllowProjectile() const { return m_AllowProjectile; }

    //-------------------------------------------------------------------------
    // Tile Operations
    bool operator==( const TileDefinition& rhs ) const;
    bool operator!=( const TileDefinition& rhs ) const;

private:
    static SpriteSheet* s_TileSpriteSheet;

    std::string m_TileDefinitionName = "NO_DEFINITION_NAME";
    AABB2 m_TileSpriteUVs = AABB2::UNIT_BOX;
    Rgba8 m_TileMapColor = Rgba8::CLEAR;
    Rgba8 m_TileTint = Rgba8::WHITE;
    int m_TileSpriteIndex = 0;

    bool m_AllowWalk = false;
    bool m_AllowFly = false;
    bool m_AllowSwim = false;
    bool m_AllowSight = false;
    bool m_AllowProjectile = false;

    TileDefinition( const XmlElement& element );
    TileDefinition( const std::string& name ); // Only used for the DefaultDefinition
};