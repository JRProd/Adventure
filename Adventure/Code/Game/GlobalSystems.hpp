#pragma once

class AudioSystem;
class InputSystem;
class RenderContext;
class RandomNumberGenerator;

//-----------------------------------------------------------------------------
// Global advertisements for engine systems
extern AudioSystem* g_AudioSystem;
extern InputSystem* g_InputSystem;
extern RenderContext* g_Renderer;
extern RandomNumberGenerator* g_GlobalSeedGenerator;