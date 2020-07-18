#pragma once

#include <vector>

#include "Engine/Core/Math/Primatives/Vec2.hpp"

#include "Game/Entity/Entity2D.hpp"
#include "Engine/Core/Rgba8.hpp"

struct Vertex_PCU;

class Debris: public Entity2D
{
    friend class DebrisType;

public:
    ~Debris();

    virtual void Create() override;
    virtual void Update( float deltaSeconds ) override;
    virtual void Render() const override;
    virtual void Die() override;
    virtual void Destroy() override;

private:
    std::vector<Vertex_PCU> m_Visual;

    float m_LifeSpan = 1.f;
    bool m_FadeOut = true;

    int m_TriangleCount = 4;
    float m_InnerRadius = .5f;
    float m_OuterRadius = 1.f;

    Rgba8 m_DebrisColor = Rgba8::MAGENTA;

    Debris( Game* gameInstance, 
            Map* mapInstance,
            const Vec2& startingPosition,
            const Vec2& initialVelocity,
            float angularVelocity,
            Vec2 scale,
            float lifeSpan,
            const Rgba8& color,
            bool fadeOut,
            int triangleCount,
            float innerRadius,
            float outerRadius) ;

};

class DebrisType
{
public:
    static DebrisType UniformDebrisExplosion( Game* gameInstance,
                                              Map* mapInstance,
                                              const Vec2& centerPointSpawn, 
                                              float initialSpeed);
    static DebrisType ShotgunWithVelocity( Game* gameInstance,
                                           Map* mapInstance,
                                           const Vec2& centerPointSpawn, 
                                           float initialSpeed, 
                                           const Vec2& direction, 
                                           float angularDistance);

    DebrisType( Game* gameInstance, 
                Map* mapInstance, 
                const Vec2& centerPointSpawn );

    DebrisType& SetCircularSpawnRadius( float circularRadius );
    DebrisType& SetInitialSpeed( float initialSpeed );
    DebrisType& SetSpeedRangeChange( float lowerRange, float upperRange );
    DebrisType& SetVelocityDirection( float initialDirection );
    DebrisType& SetInitialScale( const Vec2& initialScale );
    DebrisType& SetScaleRangeChange( float lowerScaleRange, float upperScaleRange );
    DebrisType& SetVelocityDirectionRangeChange( float lowerAngularDisplacement, float upperAngularDisplacement );
    DebrisType& SetInitialAngularVelocity( float angularVelocity );
    DebrisType& SetAngularVelocityRangeChange( float lowerAngularVelocity, float upperAngularVelocity );
    DebrisType& SetLifeSpan( float lifeSpan );
    DebrisType& SetInitialColor( const Rgba8& color );
    DebrisType& SetFadeOut( bool fadeOut );
    DebrisType& SetTriangeCount( int triangleCount );
    DebrisType& SetTriangleRadiusRange( float innerRadius, float outerRadius );
    Debris* BuildDebris();
private:
    Game* m_GameInstance = nullptr;
    Map* m_MapInstance = nullptr;
    Vec2 m_CenterPointSpawn = Vec2::ZERO;

    float m_CircleRadius = 0.f;
    float m_InitialSpeed = 0.f;
    Vec2 m_InitialVelocityDirection = Vec2::ZERO;
    Vec2 m_InitialScale = Vec2::ONE;
    float m_LowerScaleRange = 0.f;
    float m_UpperScaleRange = 0.f;
    float m_LowerInitialSpeedRange = 0.f;
    float m_UpperInitialSpeedRange = 0.f;
    float m_InitialAngularVelocity = 0.f;
    float m_LowerAngularDisplacement = 0.f;
    float m_UpperAngularDisplacement = 0.f;
    float m_LowerAngularVelocity = -50.f;
    float m_UpperAngularVelocity = 50.f;
    float m_LifeSpan = 0.f;
    Rgba8 m_InitialColor = Rgba8::WHITE;
    bool m_FadeOut = true;
    int m_TriangleCount = 5;
    float m_InnerRadius = .5f;
    float m_OuterRadius = 1.f;
};