#pragma once
#include "Engine/Event/EventSystem.hpp"

#include "Engine/Core/VertexTypes/VertexMaster.hpp"

class Game;
class Camera;

class Window;
extern Window* g_Window;

class App
{
public:
    App();
    ~App();
    void Startup();
    void Shutdown();
    void RunFrame();

    bool IsQuitting() const { return m_isQuitting; }
    bool HandleQuitRequested( EventArgs* );

private:
    Game* m_GameInstance = nullptr;
    bool m_isQuitting = false;

    void BeginFrame();
    void Update( float deltaSeconds );
    void Render() const;
    void EndFrame();

    void HandleUserInput();

    void RestartGame();
};