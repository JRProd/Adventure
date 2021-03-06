#include "App.hpp"

#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/Math/RandomNumberGenerator.hpp"
#include "Engine/Input//InputSystem.hpp"
#include "Engine/Renderer/RenderContext.hpp"

#include "Game/Game.hpp"

// Globally defined renderer
AudioSystem* g_AudioSystem = nullptr;
InputSystem* g_InputSystem = nullptr;
RenderContext* g_Renderer = nullptr;
RandomNumberGenerator* g_GlobalSeedGenerator = nullptr;



//-----------------------------------------------------------------------------
App::App()
{
}

//-----------------------------------------------------------------------------
App::~App()
{
}

//-----------------------------------------------------------------------------
void App::Startup()
{
    // Initialize the Engine
    g_AudioSystem = new AudioSystem();

    g_InputSystem = &InputSystem::INSTANCE();
    g_InputSystem->SetWindow( g_Window );

    g_Renderer = new RenderContext();
    g_Renderer->Startup( g_Window );

    g_GlobalSeedGenerator = new RandomNumberGenerator();

    g_Console->Log( LOG_INFO, "App is starting up" );

    g_EventSystem->Subscribe( "WM_CLOSE", this, &App::HandleQuitRequested );
    g_EventSystem->Subscribe( "Shutdown", this, &App::HandleQuitRequested );
    
    // Initialize the Game
    m_GameInstance = new Game();
    m_GameInstance->Startup();
}

//-----------------------------------------------------------------------------
void App::Shutdown()
{
    m_GameInstance->Shutdown();
    delete m_GameInstance;
    m_GameInstance = nullptr;

    g_Renderer->Shutdown();
    delete g_Renderer;
    g_Renderer = nullptr;

    g_InputSystem = nullptr;

    delete g_AudioSystem;
    g_AudioSystem = nullptr;

    g_Console->Shutdown();
    delete g_Console;
    g_Console = nullptr;
}

//-----------------------------------------------------------------------------
void App::RunFrame()
{
    static double timeLastFrameStarted = GetCurrentTimeSeconds();
    double timeThisFrameStarted = GetCurrentTimeSeconds();
    float deltaSeconds = static_cast<float>(timeThisFrameStarted -
                                             timeLastFrameStarted);
    timeLastFrameStarted = timeThisFrameStarted;

    constexpr float MAX_DELTA_SECONDS = 1.f / 10.f;
    if ( deltaSeconds > MAX_DELTA_SECONDS ) { deltaSeconds = MAX_DELTA_SECONDS; }

    BeginFrame(); // For all engine systems, before game updates
    Update( deltaSeconds );
    Render();
    EndFrame(); // For all engine systems, after the game updates
}

//-----------------------------------------------------------------------------
bool App::HandleQuitRequested( EventArgs* )
{
    m_isQuitting = true;
    return true;
}

//-----------------------------------------------------------------------------
void App::BeginFrame()
{
    g_AudioSystem->BeginFrame();
    g_InputSystem->BeginFrame();
    g_Renderer->BeginFrame();
}

//-----------------------------------------------------------------------------
void App::Update( float deltaSeconds )
{
    HandleUserInput();

    m_GameInstance->Update( deltaSeconds );
}

//-----------------------------------------------------------------------------
void App::Render() const
{
    m_GameInstance->Render();
}

//-----------------------------------------------------------------------------
void App::EndFrame()
{
    g_Renderer->EndFrame();
    g_InputSystem->EndFrame();
    g_AudioSystem->EndFrame();
}

//-----------------------------------------------------------------------------
void App::HandleUserInput()
{
    if ( g_InputSystem->WasKeyJustPressed( GRAVE ) )
    {
        g_Console->IncrementConsoleView();
    }
    if ( g_Console->IsActive() )
    {
        if ( g_InputSystem->WasKeyJustPressed( PLUS ) )
        {
            g_Console->IncrementConsoleLogLevel();
        }
    }

    if ( g_InputSystem->WasKeyJustPressed( F8 ) )
    {
        RestartGame();
    }

    if ( g_InputSystem->WasKeyJustPressed( ESC ) )
    {
        g_EventSystem->Publish( "Shutdown" );
    }
}

//-----------------------------------------------------------------------------
void App::RestartGame()
{
    // Delete the game
    m_GameInstance->Shutdown();
    delete m_GameInstance;
    m_GameInstance = nullptr;

    // Recreate the game
    m_GameInstance = new Game();
    m_GameInstance->Startup();
}
