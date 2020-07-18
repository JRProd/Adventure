#include "Entity2D.hpp"

#include "Game/GlobalSystems.hpp"
#include "Game/Entity/EntityDefinition.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/VertexTypes/VertexMaster.hpp"
#include "Engine/Core/Math/Primatives/Disc.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/Mesh/MeshUtils.hpp"

STATIC void Entity2D::PushEntityOutOf( Entity2D& entity, const AABB2& box )
{
    Disc entityDisc = entity.GetEntityPhysicsDisc();
    if ( DoAABB2OverlapDisc( box, entityDisc ) )
    {
        entityDisc.PushOutOfAABB2Fixed( box );
    }

    entity.m_Position = entityDisc.center;
}

void Entity2D::Create()
{
}

void Entity2D::Update( float deltaSeconds )
{
    m_Age += deltaSeconds;

    m_Velocity *= m_VelocityModifier;
    m_Position += m_Velocity * deltaSeconds;
    m_Velocity += m_Acceleration * deltaSeconds;

    m_AngleDegrees += m_AngularVelocity * deltaSeconds;
    m_AngularVelocity += m_AngularAcceleration * deltaSeconds;

    m_VelocityModifier = 1.f;
}

void Entity2D::Render() const
{
}

void Entity2D::DebugRender() const
{
    std::vector<VertexMaster> debug;

    // Draw Physics Circle
    AppendDiscPerimeter( debug, Disc( m_Position, m_PhysicsRadius * m_Scale.x ),
                                   Rgba8::CYAN,
                                   .025f );

    AppendDiscPerimeter( debug, Disc( m_Position, .01f ),
                                   Rgba8::RED,
                                   .025f );

    // Draw Visual Bounds
    AABB2 scaledVisualBounds = m_VisualDrawBounds.GetScaled( m_Scale );
    scaledVisualBounds.TranslateCenter( m_Position );

    AppendAABB2Perimeter( debug, scaledVisualBounds, Rgba8::MAGENTA, .025f );

    g_Renderer->BindTexture( nullptr );
    g_Renderer->DrawVertexArray( debug );
}

void Entity2D::Die()
{
}

void Entity2D::Destroy()
{
}

const Vec2 Entity2D::GetForwardVector() const
{
    return Vec2::MakeFromPolarDegrees( m_AngleDegrees );
}

const Disc Entity2D::GetEntityPhysicsDisc() const
{
    return Disc( m_Position, m_PhysicsRadius );
}

void Entity2D::SetPosition( const Vec2& newPosition )
{
    m_Position = newPosition;
}

void Entity2D::AddPosition( const Vec2& deltaPosition )
{
    m_Position += deltaPosition;
}

void Entity2D::SetVelocity( const Vec2& newVelocity )
{
    m_Velocity = newVelocity;
}

void Entity2D::AddVelocity( const Vec2& deltaVelocity )
{
    m_Velocity += deltaVelocity;
}

void Entity2D::SetAcceleration( const Vec2& newAcceleration )
{
    m_Acceleration = newAcceleration;
}

void Entity2D::AddAcceleration( const Vec2& deltaAcceleration )
{
    m_Acceleration += deltaAcceleration;
}

void Entity2D::SetScale( const Vec2& newScale )
{
    m_Scale = newScale;
}

void Entity2D::AddScale( const Vec2& deltaScale )
{
    m_Scale += deltaScale;
}

void Entity2D::SetUniformScale( float newScale )
{
    m_Scale = Vec2( newScale, newScale );
}

void Entity2D::AddUniformScale( float deltaScale )
{
    m_Scale += Vec2( deltaScale, deltaScale );
}

void Entity2D::SetAngleDegrees( float newAngleDegrees )
{
    m_AngleDegrees = newAngleDegrees;
}

void Entity2D::AddAngleDegrees( float deltaDegrees )
{
    m_AngleDegrees += deltaDegrees;
}

void Entity2D::SetAngularVelocity( float newAngularVelocity )
{
    m_AngularVelocity = newAngularVelocity;
}

void Entity2D::AddAngularVelocity( float deltaAngularVelocity )
{
    m_AngularVelocity += deltaAngularVelocity;
}

void Entity2D::SetAngularAcceleration( float newAngularAcceleration )
{
    m_AngularAcceleration = newAngularAcceleration;
}

void Entity2D::AddAngularAcceleration( float deltaAngularAcceleration )
{
    m_AngularAcceleration += deltaAngularAcceleration;
}

void Entity2D::SetCanWalk( bool newCanWalk )
{
    m_CanWalk = newCanWalk;
}

void Entity2D::SetCanFly( bool newCanFly )
{
    m_CanFly = newCanFly;
}

void Entity2D::SetCanSwim( bool newCanSwim )
{
    m_CanSwin = newCanSwim;
}

void Entity2D::SetVelocityModifier( float velocityModifier )
{
    m_VelocityModifier = velocityModifier;
}

void Entity2D::SetHealth( int newHealth )
{
    m_Health = newHealth;
}

void Entity2D::DamageEntity( int damage )
{
    m_Health -= damage;

    if ( m_Health <= 0 )
    {
        Die();
    }
}

void Entity2D::SetDead( bool newDead )
{
    m_IsDead = newDead;
}

bool Entity2D::IsGarbage() const
{
    return m_IsGarbage;
}

Entity2D::Entity2D( Game* gameInstance, 
                    Map* mapInstance, 
                    const std::string& name, 
                    EntityDefinition& entityDefinition, 
                    const Vec2& position, 
                    float rotationDegrees, 
                    const Vec2& scale )
    : m_GameInstance( gameInstance )
    , m_MapInstance( mapInstance )
    , m_Name( name )
    , m_EntityDefintion( entityDefinition )
    , m_Position( position )
    , m_AngleDegrees( rotationDegrees )
    , m_Scale( scale )
{
    m_PhysicsRadius = m_EntityDefintion.GetPhysicsRadius();
    m_VisualDrawBounds = m_EntityDefintion.GetVisualBounds();
    
    m_Health = m_EntityDefintion.GetStartingHealth();

    m_CanWalk = m_EntityDefintion.GetCanWalk();
    m_CanFly = m_EntityDefintion.GetCanFly();
    m_CanSwin = m_EntityDefintion.GetCanSwim();
}
