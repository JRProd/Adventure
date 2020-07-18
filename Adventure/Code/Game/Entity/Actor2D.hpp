#pragma once

#include "Game/Entity/Entity2D.hpp"

#include "Engine/Input/XboxController.hpp"

class ActorDefinition;

class Actor2D: public Entity2D
{
public:
    Actor2D( Game* gameInstance,
             Map* mapInstatnce,
             const std::string& actorName,
             ActorDefinition& actorDefinition,
             const Vec2& position,
             float angleDegrees = 0.f,
             const Vec2& scale = Vec2::ONE,
             ControllerId controllerToUse = NO_CONTROLLER );

    virtual void Create() override;
    virtual void Update( float deltaSeconds ) override;
    virtual void Render() const override;
    virtual void DebugRender() const override;
    virtual void Die() override;
    virtual void Destroy() override;

private:
    ControllerId m_XboxControllerId = NO_CONTROLLER;
    ActorDefinition& m_ActorDefinition;
    std::string m_CurrentAnimState = "Idle";

    float m_Speed = 2.f;
    float m_LastDirectionChange = 0.f;
    float m_ChooseDirection = 2.f;

    void HandleUserInput( float deltaSeconds );
    void UpdateAnimState();
    void ClampVelocity();
};