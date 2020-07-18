#include "PlayerCharacter.hpp"

#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/RenderContext.hpp"

#include "Game/GameCommon.hpp"
#include "Game/GameTextureManager.hpp"

PlayerCharacter::PlayerCharacter( const Vec3& startingPosition )
    : Entity( startingPosition )
{
    m_PhysicsRadius = .3f;
    m_CosmeticRadius = .6f;

    m_EntityType = ENTITY_PLAYER;
}

void PlayerCharacter::Create()
{
    m_Texture = g_Renderer->CreateOrGetTextureFromFile( SPRITE_TANK_BODY );
    m_TurrentTexture = g_Renderer->CreateOrGetTextureFromFile( SPRITE_TANK_TURRENT );
}

void PlayerCharacter::Update( float deltaSeconds )
{
    if ( m_IsDead || m_IsGarbage ) { return; }

    HandleUserInput();

    m_AngleDegrees = GetTurnedTowards( m_AngleDegrees,
                                       m_TankTargetRotation,
                                       TANK_MAX_ROTATION_SECONDS * deltaSeconds );
    
    m_TurrentCurrentRotation = GetTurnedTowards( m_TurrentCurrentRotation,
                                                 m_TurrentTargetRotation,
                                                 TANK_TURRENT_MAX_ROTATION_SECONDS * deltaSeconds );

    Entity::Update( deltaSeconds );


    ClampVelocity();
}

void PlayerCharacter::Render() const
{
    if ( m_IsDead || m_IsGarbage ) { return; }
    RenderTankBody();
    RenderTankTurrent();
}

void PlayerCharacter::Die()
{
}

void PlayerCharacter::Destroy()
{
}

void PlayerCharacter::HandleUserInput()
{
    Vec2 tankTargetPosition = Vec2::ZERO;
    if ( g_InputSystem->IsKeyPressed( 'W' ) )
    {
        tankTargetPosition.y += 1;
    }
    if ( g_InputSystem->IsKeyPressed( 'A' ) )
    {
        tankTargetPosition.x -= 1;
    }
    if ( g_InputSystem->IsKeyPressed( 'S' ) )
    {
        tankTargetPosition.y -= 1;
    }
    if ( g_InputSystem->IsKeyPressed( 'D' ) )
    {
        tankTargetPosition.x += 1;
    }

    Vec2 turrentTargetPosition = Vec2::ZERO;
    if ( g_InputSystem->IsKeyPressed( 'I' ) )
    {
        turrentTargetPosition.y += 1;
    }
    if ( g_InputSystem->IsKeyPressed( 'J' ) )
    {
        turrentTargetPosition.x -= 1;
    }
    if ( g_InputSystem->IsKeyPressed( 'K' ) )
    {
        turrentTargetPosition.y -= 1;
    }
    if ( g_InputSystem->IsKeyPressed( 'L' ) )
    {
        turrentTargetPosition.x += 1;
    }

    if ( tankTargetPosition != Vec2::ZERO )
    {
        m_TankTargetRotation = tankTargetPosition.GetAngleDegrees();
        SetVelocity( Vec3::MakeFromPolarDegreesXY( m_AngleDegrees, TANK_MAX_VELOCITY ) );
    }
    else
    {
        m_TankTargetRotation = m_AngleDegrees;
        SetVelocity( Vec3::ZERO );
    }

    if ( turrentTargetPosition != Vec2::ZERO )
    {
        m_TurrentTargetRotation = turrentTargetPosition.GetAngleDegrees();
    }
    else
    {
        m_TurrentTargetRotation = m_TurrentCurrentRotation;
    }

    HandleJoystickInput();
}

void PlayerCharacter::HandleJoystickInput()
{
    if ( g_InputSystem->GetXboxController( 0 ).IsConnected() )
    {
        const XboxController& gamepad = g_InputSystem->GetXboxController( 0 );

        if ( gamepad.GetLeftJoystick().GetMagnitude() > 0 )
        {
            const AnalogJoystick& leftJoystick = gamepad.GetLeftJoystick();

            m_TankTargetRotation = leftJoystick.GetAngleDegrees();

            SetVelocity( Vec3::MakeFromPolarDegreesXY( m_AngleDegrees,
                                                       TANK_MAX_VELOCITY * leftJoystick.GetMagnitude() )
            );
        }
        else if ( !g_InputSystem->IsAnyKeyDown() )
        {
            m_TankTargetRotation = m_AngleDegrees;

            SetVelocity( Vec3::ZERO );
        }

        if ( gamepad.GetRightJoystick().GetMagnitude() > 0 )
        {
            const AnalogJoystick& rightJoystick = gamepad.GetRightJoystick();

            m_TurrentTargetRotation = rightJoystick.GetAngleDegrees();
        }
        else if ( !g_InputSystem->IsAnyKeyDown() )
        {
            m_TurrentTargetRotation = m_TurrentCurrentRotation;
        }
    }
}

void PlayerCharacter::ClampVelocity()
{
    m_Velocity.ClampLength( TANK_MAX_VELOCITY );
}

void PlayerCharacter::RenderTankBody() const
{
    std::vector<Vertex_PCU> bodyVisual;
    AppendAABB2ToVectorPCU( bodyVisual, m_BoundingBoxUnits, Rgba8::WHITE );

    TransformVertexArray( bodyVisual, m_AngleDegrees, m_Position );
    g_Renderer->BindTexture( m_Texture );
    g_Renderer->DrawVertexArray( bodyVisual );
}

void PlayerCharacter::RenderTankTurrent() const
{
    std::vector<Vertex_PCU> turrentVisual;
    AppendAABB2ToVectorPCU( turrentVisual, m_TurrentBoundingBoxUnits, Rgba8::WHITE );

    Vec3 correctedPosition = m_Position;
    correctedPosition += static_cast<Vec3>(m_TurrentVisualOffset.GetRotatedDegrees( m_TurrentCurrentRotation ));

    TransformVertexArray( turrentVisual, m_TurrentCurrentRotation, correctedPosition );
    g_Renderer->BindTexture( m_TurrentTexture );
    g_Renderer->DrawVertexArray( turrentVisual );
}
