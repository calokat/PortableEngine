#ifdef __EMSCRIPTEN__
#include "EmscriptenInputSystem.h"
#include <iostream>
#include <imgui.h>
#include <SDL/SDL.h>
#include <examples/imgui_impl_sdl.h>

InputData EmscriptenInputSystem::current;
InputData EmscriptenInputSystem::previous;
glm::vec2 EmscriptenInputSystem::deltaCursorPos;

EmscriptenInputSystem::EmscriptenInputSystem()
{
	// emscripten_set_keydown_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::KeyDownCallback);
	// emscripten_set_keyup_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::KeyUpCallback);
	// emscripten_set_mousemove_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseCallback);
	// emscripten_set_mousedown_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseDownCallback);
	// emscripten_set_mouseup_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseUpCallback);
	//sdlKeyMap.emplace<int, int>(97, KeyboardCode::A);
	//sdlKeyMap.emplace<int, int>(98, KeyboardCode::B);
	//sdlKeyMap.emplace<int, int>(99, KeyboardCode::C);
	//sdlKeyMap.emplace<int, int>(100, KeyboardCode::D);
	//sdlKeyMap.emplace<int, int>(101, KeyboardCode::E);
	//sdlKeyMap.emplace<int, int>(102, KeyboardCode::F);
	//sdlKeyMap.emplace<int, int>(103, KeyboardCode::G);
	//sdlKeyMap.emplace<int, int>(104, KeyboardCode::H);
	//sdlKeyMap.emplace<int, int>(105, KeyboardCode::I);
	//sdlKeyMap.emplace<int, int>(106, KeyboardCode::J);
	//sdlKeyMap.emplace<int, int>(107, KeyboardCode::K);
	//sdlKeyMap.emplace<int, int>(108, KeyboardCode::L);
	//sdlKeyMap.emplace<int, int>(109, KeyboardCode::M);
	//sdlKeyMap.emplace<int, int>(110, KeyboardCode::N);
	//sdlKeyMap.emplace<int, int>(111, KeyboardCode::O);
	//sdlKeyMap.emplace<int, int>(112, KeyboardCode::P);
	//sdlKeyMap.emplace<int, int>(113, KeyboardCode::Q);
	//sdlKeyMap.emplace<int, int>(114, KeyboardCode::R);
	//sdlKeyMap.emplace<int, int>(115, KeyboardCode::S);
	//sdlKeyMap.emplace<int, int>(116, KeyboardCode::T);
	//sdlKeyMap.emplace<int, int>(117, KeyboardCode::U);
	//sdlKeyMap.emplace<int, int>(118, KeyboardCode::V);
	//sdlKeyMap.emplace<int, int>(119, KeyboardCode::W);
	//sdlKeyMap.emplace<int, int>(120, KeyboardCode::X);
	//sdlKeyMap.emplace<int, int>(121, KeyboardCode::Y);
	//sdlKeyMap.emplace<int, int>(122, KeyboardCode::Z);
}

void EmscriptenInputSystem::GetKeyPressed()
{
	previous = current;
	SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        // Capture events here, based on io.WantCaptureMouse and io.WantCaptureKeyboard
		//printf("Key is %i and KeyCode is %i\n", event.key.keysym.scancode, sdlKeyMap[event.key.keysym.scancode]);
		//current.keys[sdlKeyMap[event.key.keysym.scancode]] = (event.type == SDL_KEYDOWN);

		KeyboardCode keyCode;
		switch (event.key.keysym.sym)
		{
		case SDL_SCANCODE_0:
			keyCode = KeyboardCode::Num0;
			break;
		case SDL_SCANCODE_1:
			keyCode = KeyboardCode::Num1;
			break;
		case SDL_SCANCODE_2:
			keyCode = KeyboardCode::Num2;
			break;
		case SDL_SCANCODE_3:
			keyCode = KeyboardCode::Num3;
			break;
		case SDL_SCANCODE_4:
			keyCode = KeyboardCode::Num4;
			break;
		case SDL_SCANCODE_5:
			keyCode = KeyboardCode::Num5;
			break;
		case SDL_SCANCODE_6:
			keyCode = KeyboardCode::Num6;
			break;
		case SDL_SCANCODE_7:
			keyCode = KeyboardCode::Num7;
			break;
		case SDL_SCANCODE_8:
			keyCode = KeyboardCode::Num8;
			break;
		case SDL_SCANCODE_9:
			keyCode = KeyboardCode::Num9;
			break;
		case SDL_SCANCODE_A:
			keyCode = KeyboardCode::A;
			break;
		case SDL_SCANCODE_B:
			keyCode = KeyboardCode::B;
			break;
		case SDL_SCANCODE_C:
			keyCode = KeyboardCode::C;
			break;
		case SDL_SCANCODE_D:
			keyCode = KeyboardCode::D;
			break;
		case SDL_SCANCODE_E:
			keyCode = KeyboardCode::E;
			break;
		case SDL_SCANCODE_F:
			keyCode = KeyboardCode::F;
			break;
		case SDL_SCANCODE_G:
			keyCode = KeyboardCode::G;
			break;
		case SDL_SCANCODE_H:
			keyCode = KeyboardCode::H;
			break;
		case SDL_SCANCODE_I:
			keyCode = KeyboardCode::I;
			break;
		case SDL_SCANCODE_J:
			keyCode = KeyboardCode::J;
			break;
		case SDL_SCANCODE_K:
			keyCode = KeyboardCode::K;
			break;
		case SDL_SCANCODE_L:
			keyCode = KeyboardCode::L;
			break;
		case SDL_SCANCODE_M:
			keyCode = KeyboardCode::M;
			break;
		case SDL_SCANCODE_N:
			keyCode = KeyboardCode::N;
			break;
		case SDL_SCANCODE_O:
			keyCode = KeyboardCode::O;
			break;
		case SDL_SCANCODE_P:
			keyCode = KeyboardCode::P;
			break;
		case SDL_SCANCODE_Q:
			keyCode = KeyboardCode::Q;
			break;
		case SDL_SCANCODE_R:
			keyCode = KeyboardCode::R;
			break;
		case SDL_SCANCODE_S:
			keyCode = KeyboardCode::S;
			break;
		case SDL_SCANCODE_T:
			keyCode = KeyboardCode::T;
			break;
		case SDL_SCANCODE_U:
			keyCode = KeyboardCode::U;
			break;
		case SDL_SCANCODE_V:
			keyCode = KeyboardCode::V;
			break;
		case SDL_SCANCODE_W:
			keyCode = KeyboardCode::W;
			break;
		case SDL_SCANCODE_X:
			keyCode = KeyboardCode::X;
			break;
		case SDL_SCANCODE_Y:
			keyCode = KeyboardCode::Y;
			break;
		case SDL_SCANCODE_Z:
			keyCode = KeyboardCode::Z;
			break;
		default:
			keyCode = KeyboardCode::N;
		}
		current.keys[keyCode] = event.type == SDL_KEYDOWN;
    }
	ImGuiIO& io = ImGui::GetIO();
	current.mouseButtons[MouseButton::Left] = io.MouseDown[0];
	current.mouseButtons[MouseButton::Right] = io.MouseDown[1];
	current.cursorPos = {io.MousePos.x, io.MousePos.y};
	// int mx, my;
	// SDL_GetMouseState(&mx, &my);
	// current.cursorPos.x = (float)mx; current.cursorPos.y = (float)my;
	//current.keys[KeyboardCode::W] = true;

}
bool EmscriptenInputSystem::IsKeyPressed(KeyboardCode kc)
{
	return current.keys[kc];
}

bool EmscriptenInputSystem::WasKeyPressed(KeyboardCode kc)
{
	return previous.keys[kc];
}

bool EmscriptenInputSystem::IsMouseButtonPressed(MouseButton mb)
{
	return current.mouseButtons[mb];
}

bool EmscriptenInputSystem::WasMouseButtonPressed(MouseButton mb)
{
	return previous.mouseButtons[mb];
}

bool EmscriptenInputSystem::WasMouseButtonClicked(MouseButton mb)
{
	return previous.mouseButtons[MouseButton::Left] && !current.mouseButtons[MouseButton::Left];
}

glm::vec2 EmscriptenInputSystem::GetCursorPosition()
{
	return current.cursorPos;
}
glm::vec2 EmscriptenInputSystem::GetPreviousCursorPosition()
{
	return previous.cursorPos;
}

glm::vec2 EmscriptenInputSystem::GetDeltaCursorPosition()
{
	// Hack: deltaCursorPos does not become (0, 0) when the mouse stops moving. When the mouse stops moving, MouseCallback is not called,
	// so deltaCursorPos is not assigned (0, 0) and it retains its non-zero value from the previous frame. This hack ensures that 
	// deltaCursorPos is (0, 0) when the mouse is not moving by manually zeroing it and letting that value be overridden in MouseCallback
	// when called.
	// glm::vec2 returnValue = deltaCursorPos;
	// deltaCursorPos = glm::vec2(0, 0);
	return current.cursorPos - previous.cursorPos;
}
EM_BOOL EmscriptenInputSystem::MouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	// previous = current;
	current.cursorPos = glm::vec2(mouseEvent->targetX, mouseEvent->targetY);
	deltaCursorPos = glm::vec2(mouseEvent->movementX, mouseEvent->movementY);
	return false;
}
EM_BOOL EmscriptenInputSystem::MouseDownCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	ImGuiIO& io = ImGui::GetIO();
	if ((mouseEvent->buttons & 2) == 2)
	{
		io.MouseDown[2] = true;
		current.mouseButtons[MouseButton::Right] = true;
		emscripten_request_pointerlock("canvas.emscripten", true);
	}
	if ((mouseEvent->button) == 0)
	{
		io.MouseDown[0] = true;
		current.mouseButtons[MouseButton::Left] = true;
	}
	return false;
}
EM_BOOL EmscriptenInputSystem::MouseUpCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	ImGuiIO& io = ImGui::GetIO();
	if ((mouseEvent->buttons & 2) != 2)
	{
		io.MouseDown[2] = false;
		current.mouseButtons[MouseButton::Right] = false;
		emscripten_exit_pointerlock();
	}
	if ((mouseEvent->button) == 0)
	{
		io.MouseDown[0] = false;
		current.mouseButtons[MouseButton::Left] = false;
	}
	return false;
}
EM_BOOL EmscriptenInputSystem::KeyDownCallback(int eventType, const EmscriptenKeyboardEvent* kbEvent, void* userData)
{
	ImGuiIO& io = ImGui::GetIO();
	// for now, only add input characters if they are printable(i.e. not whitespace or special characters)
	// special characters have key strings of more than one character
	if (strlen(kbEvent->key) == 1)
	{
		io.AddInputCharacterUTF16((unsigned short)kbEvent->key[0]);
	}
	current.keys[KeyboardCode::A] = strcmp(kbEvent->key, "a") == 0;
	current.keys[KeyboardCode::B] = strcmp(kbEvent->key, "b") == 0;
	current.keys[KeyboardCode::C] = strcmp(kbEvent->key, "c") == 0;
	current.keys[KeyboardCode::D] = strcmp(kbEvent->key, "d") == 0;
	current.keys[KeyboardCode::E] = strcmp(kbEvent->key, "e") == 0;
	current.keys[KeyboardCode::F] = strcmp(kbEvent->key, "f") == 0;
	current.keys[KeyboardCode::G] = strcmp(kbEvent->key, "g") == 0;
	current.keys[KeyboardCode::H] = strcmp(kbEvent->key, "h") == 0;
	current.keys[KeyboardCode::I] = strcmp(kbEvent->key, "i") == 0;
	current.keys[KeyboardCode::J] = strcmp(kbEvent->key, "j") == 0;
	current.keys[KeyboardCode::K] = strcmp(kbEvent->key, "k") == 0;
	current.keys[KeyboardCode::L] = strcmp(kbEvent->key, "l") == 0;
	current.keys[KeyboardCode::M] = strcmp(kbEvent->key, "m") == 0;
	current.keys[KeyboardCode::N] = strcmp(kbEvent->key, "n") == 0;
	current.keys[KeyboardCode::O] = strcmp(kbEvent->key, "o") == 0;
	current.keys[KeyboardCode::P] = strcmp(kbEvent->key, "p") == 0;
	current.keys[KeyboardCode::Q] = strcmp(kbEvent->key, "q") == 0;
	current.keys[KeyboardCode::R] = strcmp(kbEvent->key, "r") == 0;	
	current.keys[KeyboardCode::S] = strcmp(kbEvent->key, "s") == 0;
	current.keys[KeyboardCode::T] = strcmp(kbEvent->key, "t") == 0;
	current.keys[KeyboardCode::U] = strcmp(kbEvent->key, "u") == 0;
	current.keys[KeyboardCode::V] = strcmp(kbEvent->key, "v") == 0;
	current.keys[KeyboardCode::W] = strcmp(kbEvent->key, "w") == 0;
	current.keys[KeyboardCode::X] = strcmp(kbEvent->key, "x") == 0;
	current.keys[KeyboardCode::Y] = strcmp(kbEvent->key, "y") == 0;	
	current.keys[KeyboardCode::Z] = strcmp(kbEvent->key, "z") == 0;
	current.keys[KeyboardCode::Num0] = strcmp(kbEvent->key, "0") == 0;
	current.keys[KeyboardCode::Num1] = strcmp(kbEvent->key, "1") == 0;
	current.keys[KeyboardCode::Num2] = strcmp(kbEvent->key, "2") == 0;
	current.keys[KeyboardCode::Num3] = strcmp(kbEvent->key, "3") == 0;
	current.keys[KeyboardCode::Num4] = strcmp(kbEvent->key, "4") == 0;
	current.keys[KeyboardCode::Num5] = strcmp(kbEvent->key, "5") == 0;
	current.keys[KeyboardCode::Num6] = strcmp(kbEvent->key, "6") == 0;
	current.keys[KeyboardCode::Num7] = strcmp(kbEvent->key, "7") == 0;
	current.keys[KeyboardCode::Num8] = strcmp(kbEvent->key, "8") == 0;
	current.keys[KeyboardCode::Num9] = strcmp(kbEvent->key, "9") == 0;
	current.keys[KeyboardCode::LCtrl] = strcmp(kbEvent->key, "Control") == 0;
	current.keys[KeyboardCode::RCtrl] = strcmp(kbEvent->key, "Control") == 0;
	current.keys[KeyboardCode::LShift] = strcmp(kbEvent->key, "Shift") == 0;
	current.keys[KeyboardCode::RShift] = strcmp(kbEvent->key, "Shift") == 0;
	current.keys[KeyboardCode::ForwardSlash] = strcmp(kbEvent->key, "/") == 0;
	current.keys[KeyboardCode::BackSlash] = strcmp(kbEvent->key, "\\") == 0;
	current.keys[KeyboardCode::Enter] = strcmp(kbEvent->key, "Enter") == 0;
	current.keys[KeyboardCode::Esc] = strcmp(kbEvent->key, "Escape") == 0;
	return false;
}

EM_BOOL EmscriptenInputSystem::KeyUpCallback(int eventType, const EmscriptenKeyboardEvent* kbEvent, void* userData)
{
	current.keys[KeyboardCode::A] = (current.keys[KeyboardCode::A] && strcmp(kbEvent->key, "a") != 0);
	current.keys[KeyboardCode::B] = (current.keys[KeyboardCode::B] && strcmp(kbEvent->key, "b") != 0);
	current.keys[KeyboardCode::C] = (current.keys[KeyboardCode::C] && strcmp(kbEvent->key, "c") != 0);
	current.keys[KeyboardCode::D] = (current.keys[KeyboardCode::D] && strcmp(kbEvent->key, "d") != 0);
	current.keys[KeyboardCode::E] = (current.keys[KeyboardCode::E] && strcmp(kbEvent->key, "e") != 0);
	current.keys[KeyboardCode::F] = (current.keys[KeyboardCode::F] && strcmp(kbEvent->key, "f") != 0);
	current.keys[KeyboardCode::G] = (current.keys[KeyboardCode::G] && strcmp(kbEvent->key, "g") != 0);
	current.keys[KeyboardCode::H] = (current.keys[KeyboardCode::H] && strcmp(kbEvent->key, "h") != 0);
	current.keys[KeyboardCode::I] = (current.keys[KeyboardCode::I] && strcmp(kbEvent->key, "i") != 0);
	current.keys[KeyboardCode::J] = (current.keys[KeyboardCode::J] && strcmp(kbEvent->key, "j") != 0);
	current.keys[KeyboardCode::K] = (current.keys[KeyboardCode::K] && strcmp(kbEvent->key, "k") != 0);
	current.keys[KeyboardCode::L] = (current.keys[KeyboardCode::L] && strcmp(kbEvent->key, "l") != 0);
	current.keys[KeyboardCode::M] = (current.keys[KeyboardCode::M] && strcmp(kbEvent->key, "m") != 0);
	current.keys[KeyboardCode::N] = (current.keys[KeyboardCode::N] && strcmp(kbEvent->key, "n") != 0);
	current.keys[KeyboardCode::O] = (current.keys[KeyboardCode::O] && strcmp(kbEvent->key, "o") != 0);
	current.keys[KeyboardCode::P] = (current.keys[KeyboardCode::P] && strcmp(kbEvent->key, "p") != 0);
	current.keys[KeyboardCode::Q] = (current.keys[KeyboardCode::Q] && strcmp(kbEvent->key, "q") != 0);
	current.keys[KeyboardCode::R] = (current.keys[KeyboardCode::R] && strcmp(kbEvent->key, "r") != 0);	
	current.keys[KeyboardCode::S] = (current.keys[KeyboardCode::S] && strcmp(kbEvent->key, "s") != 0);
	current.keys[KeyboardCode::T] = (current.keys[KeyboardCode::T] && strcmp(kbEvent->key, "t") != 0);
	current.keys[KeyboardCode::U] = (current.keys[KeyboardCode::U] && strcmp(kbEvent->key, "u") != 0);
	current.keys[KeyboardCode::V] = (current.keys[KeyboardCode::V] && strcmp(kbEvent->key, "v") != 0);
	current.keys[KeyboardCode::W] = (current.keys[KeyboardCode::W] && strcmp(kbEvent->key, "w") != 0);
	current.keys[KeyboardCode::X] = (current.keys[KeyboardCode::X] && strcmp(kbEvent->key, "x") != 0);
	current.keys[KeyboardCode::Y] = (current.keys[KeyboardCode::Y] && strcmp(kbEvent->key, "y") != 0);	
	current.keys[KeyboardCode::Z] = (current.keys[KeyboardCode::Z] && strcmp(kbEvent->key, "z") != 0);
	current.keys[KeyboardCode::Num0] = (current.keys[KeyboardCode::Num0] && strcmp(kbEvent->key, "0") != 0);
	current.keys[KeyboardCode::Num1] = (current.keys[KeyboardCode::Num1] && strcmp(kbEvent->key, "1") != 0);
	current.keys[KeyboardCode::Num2] = (current.keys[KeyboardCode::Num2] && strcmp(kbEvent->key, "2") != 0);
	current.keys[KeyboardCode::Num3] = (current.keys[KeyboardCode::Num3] && strcmp(kbEvent->key, "3") != 0);
	current.keys[KeyboardCode::Num4] = (current.keys[KeyboardCode::Num4] && strcmp(kbEvent->key, "4") != 0);
	current.keys[KeyboardCode::Num5] = (current.keys[KeyboardCode::Num5] && strcmp(kbEvent->key, "5") != 0);
	current.keys[KeyboardCode::Num6] = (current.keys[KeyboardCode::Num6] && strcmp(kbEvent->key, "6") != 0);
	current.keys[KeyboardCode::Num7] = (current.keys[KeyboardCode::Num7] && strcmp(kbEvent->key, "7") != 0);
	current.keys[KeyboardCode::Num8] = (current.keys[KeyboardCode::Num8] && strcmp(kbEvent->key, "8") != 0);
	current.keys[KeyboardCode::Num9] = (current.keys[KeyboardCode::Num9] && strcmp(kbEvent->key, "9") != 0);
	current.keys[KeyboardCode::LCtrl] = (current.keys[KeyboardCode::LCtrl] && strcmp(kbEvent->key, "Control") != 0);
	current.keys[KeyboardCode::RCtrl] = (current.keys[KeyboardCode::RCtrl] && strcmp(kbEvent->key, "Control") != 0);
	current.keys[KeyboardCode::LShift] = (current.keys[KeyboardCode::LShift] && strcmp(kbEvent->key, "Shift") != 0);
	current.keys[KeyboardCode::RShift] = (current.keys[KeyboardCode::RShift] && strcmp(kbEvent->key, "Shift") != 0);
	current.keys[KeyboardCode::ForwardSlash] = (current.keys[KeyboardCode::ForwardSlash] && strcmp(kbEvent->key, "/") != 0);
	current.keys[KeyboardCode::BackSlash] = (current.keys[KeyboardCode::BackSlash] && strcmp(kbEvent->key, "\\") != 0);
	current.keys[KeyboardCode::Enter] = (current.keys[KeyboardCode::Enter] && strcmp(kbEvent->key, "Enter") != 0);
	current.keys[KeyboardCode::Esc] = (current.keys[KeyboardCode::Esc] && strcmp(kbEvent->key, "Escape") != 0);
	return false;
}
#endif