#include "Game/Entity/Actor2D.hpp"

#include "Game/GlobalSystems.hpp"
#include "Game/Game.hpp"
#include "Game/Entity/EntityDefinition.hpp"
#include "Game/Entity/ActorDefinition.hpp"

#include "Engine/Core/Time.hpp"
#include "Engine/Core/Utils/VectorPcuUtils.hpp"
#include "Engine/Core/VertexTypes/VertexMaster.hpp"
#include "Engine/Core/Math/RandomNumberGenerator.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/Mesh/MeshUtils.hpp"
#include "Engine/Renderer/Sprite/SpriteAnimSet.hpp"

Actor2D::Actor2D( Game* gameInstance,
                  Map* mapInstatnce,
                  const std::string& actorName,
                  ActorDefinition& actorDefinition,
                  const Vec2& position,
                  float angleDegrees,
                  const Vec2& scale,
                  ControllerId controllerToUse )
    : Entity2D( gameInstance, mapInstatnce, actorName, (EntityDefinition&) actorDefinition, position, angleDegrees, scale )
    , m_ActorDefinition( actorDefinition )
    , m_XboxControllerId( controllerToUse )
{
}

void Actor2D::Create()
{

}

void Actor2D::Update( float deltaSeconds )
{
    HandleUserInput( deltaSeconds );

    UpdateAnimState();

    ClampVelocity();
    Entity2D::Update( deltaSeconds );
}

void Actor2D::Render() const
{
    Entity2D::Render();

    std::vector<VertexMaster> visual;
    AABB2 visualBox = m_VisualDrawBounds.GetTranslation( m_Position );
    Vec2 uvMins = Vec2::ZERO;
    Vec2 uvMaxs = Vec2::ONE;

    const SpriteDefinition& currentAnim = m_ActorDefinition.GetSpriteAnimSet().GetSpriteDefAtTime( m_CurrentAnimState, m_Age );
    currentAnim.GetUVs( uvMins, uvMaxs );
    
    AppendAABB2( visual, visualBox, Rgba8::WHITE, uvMins, uvMaxs );
    g_Renderer->BindTexture( &currentAnim.GetTexture() );
    g_Renderer->DrawVertexArray( visual );
}

void Actor2D::DebugRender() const
{
    Entity2D::DebugRender();


}

void Actor2D::Die()
{
}

void Actor2D::Destroy()
{


    Entity2D::Destroy();
}

void Actor2D::HandleUserInput( float deltaSeconds )
{
    if ( m_XboxControllerId == NO_CONTROLLER )
    {
        if ( m_LastDirectionChange + m_ChooseDirection < GetCurrentTimeSeconds() )
        {
            float x = 0.f, y = 0.f;
            g_ActorBehaviorRng->RandomDirection(x, y);

            m_Velocity = Vec2( x, y ) * m_Speed;

            m_LastDirectionChange = GetCurrentTimeSeconds();
        }
    }

    const XboxController& controller = g_InputSystem->GetXboxController( m_XboxControllerId );
    if ( controller.IsConnected() )
    {
        const AnalogJoystick& leftJoystick = controller.GetLeftJoystick();
        if ( leftJoystick.GetMagnitude() > 0 )
        {
            m_Velocity = Vec2::MakeFromPolarDegrees( leftJoystick.GetAngleDegrees(), 2.f );
        }
        else
        {
            m_Velocity = Vec2::ZERO;
        }
    }
}

#include "Engine/Console/Console.hpp"
void Actor2D::UpdateAnimState()
{
    if ( m_Velocity != Vec2::ZERO )
    {
        float angleMovement = GetAngleZeroTo360(m_Velocity.GetAngleDegrees());
        if ( angleMovement > 35.f && angleMovement < 145.f )
        {
            m_CurrentAnimState = "WalkNorth";
        }
        else if ( angleMovement >= 145.f && angleMovement <= 215.f )
        {
            m_CurrentAnimState = "WalkWest";
        }
        else if ( angleMovement > 215.f && angleMovement < 325.f )
        {
            m_CurrentAnimState = "WalkSouth";
        }
        else if ( angleMovement <= 35.f || angleMovement >= 325.f )
        {
            m_CurrentAnimState = "WalkEast";
        }
        else
        {
            m_CurrentAnimState = "Idle";
        }
    }
    else
    {
        m_CurrentAnimState = "Idle";
    }
}

void Actor2D::ClampVelocity()
{
    m_Velocity.ClampLength( 5.f );
}


