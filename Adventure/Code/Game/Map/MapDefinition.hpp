#pragma once

#include <map>

#include "Engine/Core/Math/Primatives/IntVec2.hpp"
#include "Engine/Core/Utils/XmlUtils.hpp"

class RandomNumberGenerator;

#include "Game/Map/Tile/TileDefinition.hpp"
#include "Game/Map/MapGeneration/MapGenerationStep.hpp"

class Map;

class MapDefinition;
typedef std::map<const std::string, MapDefinition*> MapDefinitions;

class MapDefinition
{
public:
    static MapDefinitions Definitions;

    static void PopulateFromXmlElement( const XmlElement& elementRoot );
    static MapDefinition& GetFromName( const std::string& name);

    MapDefinition( const MapDefinition& copy ) = default;

    //-------------------------------------------------------------------------
    // Map Generation
    void RunGenerationSteps( Map& defaultFilledMap, RandomNumberGenerator& rng ) const;

    //-------------------------------------------------------------------------
    // Map Definition Accessors
    const std::string GetMapDefName() const { return m_MapDefinitionName; }
    const IntVec2 GetMapDefSize() const { return m_MapSize; }
    TileDefinition& GetMapDefDefualtFillTile() const { return *m_DefaultFillTile; }
    TileDefinition& GetMapDefBoundaryTile() const { return *m_BoundaryTile; }
    TileDefinition& GetMapDefInaccesableTile() const { return *m_InaccesableTile; }

private:
    std::string m_MapDefinitionName = "NO_DEFINITION_NAME";
    IntVec2 m_MapSize = IntVec2::ZERO;

    TileDefinition* m_DefaultFillTile = nullptr;
    TileDefinition* m_BoundaryTile = nullptr;
    TileDefinition* m_InaccesableTile = nullptr;

    MapGenerationSteps m_GenerationSteps;

    MapDefinition( const XmlElement& element );

    void ParseMapStartingTiles( const XmlElement& startingTileElement );
    void ParseMapGenerationSteps( const XmlElement* generationElement );
};