#pragma once

#include "Engine/Core/STL/RapidReplaceVector.hpp"
#include "Engine/Core/Math/Primatives/AABB2.hpp"
#include "Engine/Core/Math/Primatives/Vec2.hpp"

class Game;
class Map;
class EntityDefinition;

class Entity2D;
typedef RapidReplaceVector<Entity2D*> EntityList;

class Entity2D
{
public:
    //-------------------------------------------------------------------------
    // Static Entity Physics Calculations
    static void PushEntityOutOf( Entity2D& entity, const AABB2& box );

    virtual ~Entity2D() = default;

    //-------------------------------------------------------------------------
    // Entity Lifecycle
    virtual void Create() = 0;
    virtual void Update( float deltaSeconds );
    virtual void Render() const;
    virtual void DebugRender() const;
    virtual void Die() = 0;
    virtual void Destroy();

    //-------------------------------------------------------------------------
    // Entity Vector2 queries
    const Vec2 GetPosition() const { return m_Position; }
    const Vec2 GetVelocity() const { return m_Velocity; }
    const Vec2 GetAcceleration() const { return m_Acceleration; }
    const Vec2 GetScale() const { return m_Scale; }
    const Vec2 GetForwardVector() const;

    //-------------------------------------------------------------------------
    // Entity Angular queries
    float GetAngleDegrees() const { return m_AngleDegrees; }
    float GetAngularVelocity() const { return m_AngularVelocity; }
    float GetAngularAcceleration() const { return m_AngularAcceleration; }

    //-------------------------------------------------------------------------
    // Entity Members queries
    std::string GetName() const { return m_Name; }
    bool CanWalk() const { return m_CanWalk; }
    bool CanFly() const { return m_CanFly; }
    bool CanSwim() const { return m_CanSwin; }
    float GetVelocityModifier() const { return m_VelocityModifier; }
    int GetHealth() const { return m_Health; }
    bool IsDead() const { return m_IsDead; }
    const Disc GetEntityPhysicsDisc() const;

    //-------------------------------------------------------------------------
    // Entity Vector2 modifiers
    void SetPosition( const Vec2& newPosition );
    void AddPosition( const Vec2& deltaPosition );
    void SetVelocity( const Vec2& newVelocity );
    void AddVelocity( const Vec2& deltaVelocity );
    void SetAcceleration( const Vec2& newAcceleration );
    void AddAcceleration( const Vec2& deltaAcceleration );
    void SetScale( const Vec2& newScale );
    void AddScale( const Vec2& deltaScale );
    void SetUniformScale( float newScale );
    void AddUniformScale( float deltaScale );

    //-------------------------------------------------------------------------
    // Entity Angular modifiers
    void SetAngleDegrees( float newAngleDegrees );
    void AddAngleDegrees( float deltaDegrees );
    void SetAngularVelocity( float newAngularVelocity );
    void AddAngularVelocity( float deltaAngularVelocity );
    void SetAngularAcceleration( float newAngularAcceleration );
    void AddAngularAcceleration( float deltaAngularAcceleration );

    //-------------------------------------------------------------------------
    // Entity Members modifiers
    void SetCanWalk( bool newCanWalk );
    void SetCanFly( bool newCanFly );
    void SetCanSwim( bool newCanSwim );
    void SetVelocityModifier( float velocityModifier );
    void SetHealth( int newHealth );
    void DamageEntity( int damage );
    void SetDead( bool newDead );

    //-------------------------------------------------------------------------
    // Entity Cleanup queries
    bool IsGarbage() const;

protected:
    //-------------------------------------------------------------------------
    // References to other game systems
    Game* m_GameInstance = nullptr;             // Reference to the game instance where the entity is currently in
    Map* m_MapInstance = nullptr;               // Reference to the map instance where the entity is currently in
    EntityDefinition& m_EntityDefintion;

    //-------------------------------------------------------------------------
    // Entity Name
    std::string m_Name = "";

    //-------------------------------------------------------------------------
    // Positional Members and Vec3 Quantities
    Vec2 m_Position = Vec2::ZERO;               // Position of the Entity units
    Vec2 m_Velocity = Vec2::ZERO;               // Velocity of the Entity u/s
    Vec2 m_Acceleration = Vec2::ZERO;           // Acceleration of the Entity u/s/s
    Vec2 m_Scale = Vec2::ONE;                   // Uniform scale

    //-------------------------------------------------------------------------
    // Angular Members
    float m_AngleDegrees = 0.f;                 // Angular Orientation of the Entity ( 0 is East )
    float m_AngularVelocity = 0.f;              // Angular Velocity of the Entity deg/s
    float m_AngularAcceleration = 0.f;          // Angular Acceleration of the Entity deg/s/s

    //-------------------------------------------------------------------------
    // Physics Members
    float m_PhysicsRadius = 2.f;                // Collision Radius

    //-------------------------------------------------------------------------
    // Visual Members
    AABB2 m_VisualDrawBounds = AABB2::UNIT_AROUND_ZERO;

    //-------------------------------------------------------------------------
    // Entity Members
    float m_VelocityModifier = 1.f;             // Percent of velocity to use during this update
    float m_Age = 0.f;                          // Age of the entity from spawn time
    int m_Health = 1;                           // Health of the Entity

    //-------------------------------------------------------------------------
    // Movement Members
    bool m_CanWalk = false;                     // Can the entity walk
    bool m_CanFly = false;                      // Can the entity fly
    bool m_CanSwin = false;                     // Can the entity swim

    //-------------------------------------------------------------------------
    // Cleanup Members
    bool m_IsDead = false;                      // Is the Entity Dead
    bool m_IsGarbage = false;                   // Will the Entity be Garbage Collected next Update

    Entity2D( Game* gameInstance,
              Map* mapInstance,
              const std::string& name,
              EntityDefinition& entityDefinition,
              const Vec2& position,
              float rotationDegrees = 0,
              const Vec2& scale = Vec2::ONE );
};




/*
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Math/Primatives/Disc.hpp"
#include "Engine/Core/Math/Primatives/Vec2.hpp"
#include "Engine/Core/Math/Primatives/Vec3.hpp"

class Game;
class Map;
class Texture;
class SpriteDefinition;

typedef int EntityListIndex;

enum EntityType
{
    ENTITY_INVALID = -1,

    ENTITY_ALLIED_TURRET,
    ENTITY_ENEMY_TURRET,

    ENTITY_ALLIED_TANK,
    ENTITY_ENEMY_TANK,

    ENTITY_PLAYER,

    ENTITY_BOLDER,

    ENTITY_BULLET_ALLIED,
    ENTITY_BULLET_ENEMY,

    ENTITY_EXPLOSION,
    ENTITY_EXPLOSION_BULLET,
    ENTITY_EXPLOSION_NPC,
    ENTITY_EXPLOSION_PLAYER,

    ENTITY_DEBRIS,

    NUM_ENTITY_TYPES
};

enum class EntityOverlapType
{
    NONE,

    OVERLAP_ONLY,

    PUSH_PUSH,
    PUSH_NO_PUSH,
    PUSH_FIXED,
    FIXED_NO_PUSH,
};

enum Faction
{
    FACTION_NEUTRAL,

    FACTION_PLAYER,
    FACTION_NATURE,
    FACTION_ENEMY,

    NUM_FACTION_TYPES,
};

class Entity
{
public:
    Entity( Game* gameInstance,
            const Vec3& startingPositon );
    Entity( Game* gameInstance,
            const Vec3& startingPosition,
            Faction faction );
    virtual ~Entity();

    static bool DoEntityListsOverlap( EntityListIndex l1, EntityListIndex l2 );
    static EntityOverlapType OverlapsWith( const Entity* const& entity1, const Entity* const& entity2 );
    static bool OverlapsWithTiles( const Entity* const& entity );

    //-------------------------------------------------------------------------
    // Entity Lifecycle
    virtual void Create();
    virtual void Update( float deltaSeconds );
    virtual void Render() const = 0;
    virtual void DebugRender() const;
    virtual void Die() = 0;
    virtual void Destroy();

    //-------------------------------------------------------------------------
    // Entity Vector3 queries
    const Vec3 GetPosition() const;
    const Vec3 GetVelocity() const;
    const Vec3 GetAcceleration() const;
    const Vec3 GetForwardVector() const;
    const Vec3 GetScale() const;

    //-------------------------------------------------------------------------
    // Entity Angular queries
    float GetAngleDegrees() const;
    float GetAngularVelocity() const;
    float GetAngularAcceleration() const;

    //-------------------------------------------------------------------------
    // Entity queries
    EntityType GetEntityType() const;
    Faction GetEntityFaction() const;
    float GetVelocityModifier() const;
    float GetAge() const;
    int GetHealth() const;
    bool IsDead() const;

    //-------------------------------------------------------------------------
    // Entity Renderer queries
    const Texture* GetTexture() const;
    const Rgba8 GetTint() const;

    //-------------------------------------------------------------------------
    // Entity Physics queries
    const Disc GetEntityPhysicsDisc() const;
    const Vec2 GetBoundBoxUnits() const;
    bool IsFixed() const;
    bool IsPushedByWalls() const;
    bool IsPushedByEntities() const;
    bool DoesPushEntities() const;
    bool IsHitByBullets() const;
    const Vec3 GetPhysicsDiscNormalAt( const Vec3& hitPosition );

    //-------------------------------------------------------------------------
    // Entity Cleanup queries
    bool IsGarbage() const;
//     bool IsOffscreen() const;

    void SetTexture( Texture* newTexture );

    //-------------------------------------------------------------------------
    // Entity Vector3 modifiers
    void SetPosition( const Vec3& newPosition );
    void AddPosition( const Vec3& deltaPosition );
    void SetVelocity( const Vec3& newVelocity );
    void AddVelocity( const Vec3& deltaVelocity );
    void SetAcceleration( const Vec3& newAcceleration );
    void AddAcceleration( const Vec3& deltaAcceleration );
    void SetScale( const Vec3& newScale );
    void AddScale( const Vec3& deltaScale );
    void SetUniformScale( float newScale );
    void AddUniformScale( float deltaScale );

    //-------------------------------------------------------------------------
    // Entity Angular modifiers
    void SetAngleDegrees( float newAngleDegrees );
    void AddAngleDegrees( float deltaDegrees );
    void SetAngularVelocity( float newAngularVelocity );
    void AddAngularVelocity( float deltaAngularVelocity );
    void SetAngularAcceleration( float newAngularAcceleration );
    void AddAngularAcceleration( float deltaAngularAcceleration );

    //-------------------------------------------------------------------------
    // Entity Members modifiers
    void SetVelocityModifier( float velocityModifier );
    void SetHealth( int newHealth );
    void DamageEntity( int damage );
    void SetDead( bool newDead );

protected:
    //-------------------------------------------------------------------------
    // Positional Members and Vec3 Quantities
    Vec3 m_Position = Vec3::ZERO;               // Position of the Entity units
    Vec3 m_Velocity = Vec3::ZERO;               // Velocity of the Entity u/s
    Vec3 m_Acceleration = Vec3::ZERO;           // Acceleration of the Entity u/s/s
    Vec3 m_Scale = Vec3::ONE;                   // Uniform scale

    //-------------------------------------------------------------------------
    // Angular Members
    float m_AngleDegrees = 0.f;                 // Angular Orientation of the Entity ( 0 is East )
    float m_AngularVelocity = 0.f;              // Angular Velocity of the Entity deg/s
    float m_AngularAcceleration = 0.f;          // Angular Acceleration of the Entity deg/s/s

    //-------------------------------------------------------------------------
    // Entity Members
    Game* m_GameInstance = nullptr;
    EntityType m_EntityType = ENTITY_INVALID;   // Entity type
    Faction m_EntityFaction = FACTION_NEUTRAL;
    float m_VelocityModifier = 1.f;
    float m_Age = 0.f;                          // Age of the entity from spawn time
    int m_Health = 1;                           // Health of the Entity
    bool m_IsDead = false;                      // Is the Entity Dead

    //-------------------------------------------------------------------------
    // Renderer Members
    Texture* m_Texture = nullptr;               // Texture for full image textures
    const SpriteDefinition* m_Sprite = nullptr; // Sprite definition for individual sprites
    Rgba8 m_Tint = Rgba8::MAGENTA;              // The main color of the entity

    //-------------------------------------------------------------------------
    bool m_DamageSoundInitiallized = false;
    SoundID m_DamageSound = 0;

    //-------------------------------------------------------------------------
    // Physics Members
    float m_PhysicsRadius = 2.f;               // Collision Radius
    float m_CosmeticRadius = 4.f;              // Cosmetic Radius ( no geometry outside this radius)
    Vec2 m_BoundingBoxUnits = Vec2( 1.f, 1.f ); // Bounding box for square entity collisions
    bool m_OverlapsTiles = false;
    bool m_IsPushedByWalls = false;
    bool m_OverlapsEntities = false;
    bool m_IsFixed = false;
    bool m_IsPushedByEntities = false;
    bool m_DoesPushEntities = false;
    bool m_IsHitByBullets = false;

    //-------------------------------------------------------------------------
    // Cleanup Members
    bool m_IsGarbage = false;                   // Will the Entity be Garbage Collected next Update
};
*/