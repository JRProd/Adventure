#pragma once

#include "Game/Entity/Entity.hpp"

class PlayerCharacter: public Entity
{
public:
    PlayerCharacter( const Vec3& startingPosition );

    virtual void Create() override;
    virtual void Update( float deltaSeconds ) override;
    virtual void Render() const override;
    virtual void Die() override;
    virtual void Destroy() override;

private:
    float m_TankTargetRotation = 0.f;

    float m_TurrentCurrentRotation = 0.f;
    float m_TurrentTargetRotation = 0.f;

    void HandleUserInput();
    void HandleJoystickInput();

    void ClampVelocity();

    void RenderTankBody() const;

    Texture* m_TurrentTexture = nullptr;
    Vec2 m_TurrentBoundingBoxUnits = Vec2( 1.25f, 1.f );
    Vec2 m_TurrentVisualOffset = Vec2( .05f, 0.f );
    void RenderTankTurrent() const;
};