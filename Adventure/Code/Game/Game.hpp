#pragma once

#include "Engine/Core/Rgba8.hpp"
#include "Engine/Event/EventSystem.hpp"

class Camera;
class RandomNumberGenerator;
struct Vec3;
class BitmapFont;

class World;

extern bool g_DebugMode;

extern BitmapFont* g_DefaultFont;
extern RandomNumberGenerator* g_ActorBehaviorRng;

class Game
{
public:
    Game();
    ~Game();

    static bool EventTestFunction( EventArgs* args );

    void Startup();
    void Update( float deltaSeconds );
    void Render() const;
    void Shutdown();

    RandomNumberGenerator* GetRng();

private:
    Camera* m_UICamera = nullptr;
    Camera* m_GameCamera = nullptr;
    RandomNumberGenerator* m_Rng = nullptr;
    World* m_CurrentWorld = nullptr;

    float m_GameTime = 0.f;

    bool m_FullMapView = false;
    float m_TileViewHeight = 9.f;

    bool m_IsPaused = false;
    bool m_IsSlowMo = false;
    float m_SlowMoPercentage = .1f;
    bool m_IsHyperSpeed = false;
    float m_HyperSpeedPercentage = 4.f;

    float UpdateDeltaSeconds( float deltaSeconds );
    void HandleUserInput();

    void UpdateCameraPositionToPlayerPosition();

    void DebugRender() const;
    void DebugInspector() const;

    void ErrorRecoverable( const char* message );
};
