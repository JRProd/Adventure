#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in very few places

#include "Game/App.hpp"

#include "Engine/Console/Console.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Utils/XmlUtils.hpp"
#include "Engine/Event/EventSystem.hpp"
#include "Engine/OS/Window.hpp"

//-------------------------------------------------------------------------------
// #SD1ToDo: Move each of these items to its proper place, once that place is established
const char* g_ConfigFileName = "Data/GameConfig.xml";

//-------------------------------------------------------------------------------
App* g_App = nullptr;
Window* g_Window = nullptr;

bool CommandSetWindowTitle( EventArgs* args )
{
    int numArgs = args->GetValue( "numArgs", 0 );

    std::string fullTitle;
    if( numArgs == 0 )
    {
        g_Console->InvalidArgument( "SetTitle", "Set title command requires a title after the command" );
        return false;
    }

    fullTitle += args->GetValue( "title", "" );

    g_Window->SetTitle( fullTitle );
    return true;
}

//-----------------------------------------------------------------------------
// Handles application startup event like initializing the console, loading
//  configs from GameConfig.xml and creating the OSWindow and RenderContext before
//  the app is created.
void Startup()
{
    //Initialize the event system
    g_EventSystem = new EventSystem();
    g_EventSystem->Startup();

    // Initialize the console
    g_Console = new Console();
    g_Console->Startup();

    Command setTitle;
    setTitle.commandName = "SetTitle";
    setTitle.arguments.push_back( new TypedArgument<std::string>( "title", false ) );
    setTitle.description = "Set the title of the window";

    Console::RegisterCommand( setTitle, CommandSetWindowTitle );

    // Load configuration from the game
    XmlDocument gameConfig;
    gameConfig.LoadFile( g_ConfigFileName );
    if( !gameConfig.Error() )
    {
        g_GameConfigBlackboard = NamedStrings::MakeFromXmlElement( *gameConfig.FirstChildElement() );
    }
    else
    {
        g_Console->Log( LOG_ERROR, Stringf( "Main_Windows.Startup: \"%s\" file not found", g_ConfigFileName ) );
    }

    std::string title = g_GameConfigBlackboard.GetValue( "gameTitle", "NO TITLE FOUND" );
    float clientAspect = g_GameConfigBlackboard.GetValue( "clientAspect", 16.f / 9.f );

    g_Window = new Window();
    g_Window->Open( title, clientAspect, .75f );

    // Creates the app and starts it up
    g_App = new App();
    g_App->Startup();
}

//-----------------------------------------------------------------------------
// Closes the app and all remaining engine systems and ensures any final 
//  requirements are satisfied
void Shutdown()
{
    g_App->Shutdown();
    delete g_App;
    g_App = nullptr;

    g_Console->Shutdown();
    delete g_Console;
    g_Console = nullptr;

    g_EventSystem->Shutdown();
    delete g_EventSystem;
    g_EventSystem = nullptr;
}

//-------------------------------------------------------------------------------
int WINAPI WinMain( _In_ HINSTANCE applicationInstanceHandle, _In_opt_ HINSTANCE, _In_ LPSTR commandLineString, _In_ int )
{
    UNUSED( commandLineString );
    UNUSED( applicationInstanceHandle );

    Startup();

    // Program main loop; keep running frames until it's time to quit
    while( !g_App->IsQuitting() )
    {
        g_Window->BeginFrame();
        g_App->RunFrame();
    }

    Shutdown();
    return 0;
}


