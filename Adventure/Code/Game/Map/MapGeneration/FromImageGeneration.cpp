#include "FromImageGeneration.hpp"

#include "Engine/Core/Utils/XmlUtils.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"
#include "Engine/Core/Math/Primatives/AABB2.hpp"
#include "Engine/Core/Math/RandomNumberGenerator.hpp"

#include "Game/Map/Map.hpp"

FromImageGeneration::~FromImageGeneration()
{
}

FromImageGeneration::FromImageGeneration( const XmlElement& element )
    : MapGenerationStep( element )
    , m_ImageForGeneration( ParseXmlAttribute( element, "imageFilePath" ) )
{
    m_ImageForGeneration.Create();

    std::string alignment = ParseXmlAttribute( element, "alignment", "0~1,0~1" );
    Strings uvAlignmentString = SplitStringOnDelimiter( alignment, ',' );
    m_AlignmentU = FloatRange::CreateFromString( uvAlignmentString.at( 0 ).c_str() );
    m_AlignmentV = FloatRange::CreateFromString( uvAlignmentString.at( 1 ).c_str() );

    m_ChanceToMirror = ParseXmlAttribute( element, "chanceToMirror", m_ChanceToMirror );
    m_ChancePerTile = ParseXmlAttribute( element, "chancePerTile", m_ChancePerTile );

    m_Rotations = ParseXmlAttribute( element, "rotations", m_Rotations );
}

void FromImageGeneration::RunStepOnce( Map& mapToMutate, RandomNumberGenerator& rng )
{
    // Mirror and rotate first
    bool isMirrored = rng.Chance( m_ChanceToMirror );
    int rotations = 90 * m_Rotations.GetRandomInRange( rng );

    Image rotatedImage = Image( m_ImageForGeneration, rotations, isMirrored );

    // Fit inside box
    Vec2 uvPosition = Vec2( m_AlignmentU.GetRandomInRange( rng ), m_AlignmentV.GetRandomInRange( rng ) );
    Vec2 imageMaxBottomLeft = mapToMutate.GetMapBounds().maxs - static_cast<Vec2>(rotatedImage.GetDimensions());
    Vec2 imageBottomLeft = Vec2::Lerp( uvPosition, mapToMutate.GetMapBounds().mins, imageMaxBottomLeft );
    IntVec2 tilePosBottomLeft = IntVec2( roundi( imageBottomLeft.x ), roundi( imageBottomLeft.y ) );

    // Place the image in the map
    for ( int yCoord = 0; yCoord < rotatedImage.GetDimensions().y; ++yCoord )
    {
        for ( int xCoord = 0; xCoord < rotatedImage.GetDimensions().x; ++xCoord )
        {
            IntVec2 imageCoord = IntVec2( xCoord, yCoord );
            const Rgba8& tileColor = rotatedImage.GetTexelColor( imageCoord );
            if ( rng.Chance( tileColor.GetAlphaAsFloat() * m_ChancePerTile ) )
            {
                TileDefinition& tileDef = TileDefinition::GetFromColor( tileColor );
                IntVec2 mapCoord = tilePosBottomLeft + imageCoord;

                if ( mapToMutate.IsValidTilePos( mapCoord ) )
                {
                    Tile* tile = mapToMutate.GetTileFromPosition( mapCoord );
                    tile->SetTileDefinition( &tileDef );
                }
            }
        }
    }
}
