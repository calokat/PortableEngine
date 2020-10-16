#ifdef __EMSCRIPTEN__
#pragma once
#include "GameWindow.h"
#include "IInputSystem.h"
bool ImGui_ImplEmscripten_Init(GameWindow* win, IInputSystem* in);
void ImGui_ImplEmscripten_Shutdown();
void ImGui_ImplEmscripten_NewFrame();
#endif