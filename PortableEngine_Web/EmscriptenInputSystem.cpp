#ifdef __EMSCRIPTEN__
#include "EmscriptenInputSystem.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <emscripten.h>
#include <imgui_impl_sdl.h>
InputData EmscriptenInputSystem::current;
InputData EmscriptenInputSystem::previous;
glm::vec2 EmscriptenInputSystem::deltaCursorPos;

EM_JS(void, browserCopy, (const char* toCopy), {
		let result = navigator.clipboard.writeText(UTF8ToString(toCopy)).then(() => console.log(UTF8ToString(toCopy)));
	});

EM_JS(const char*, browserPaste, (), {	
	  return Asyncify.handleAsync(async () => {
			let cbText = await navigator.clipboard.readText();
			let lengthBytes = lengthBytesUTF8(cbText) + 1;
			let stringOnWasmHeap = _malloc(lengthBytes);
			stringToUTF8(cbText, stringOnWasmHeap, lengthBytes);
			return stringOnWasmHeap;
	  })
})
EmscriptenInputSystem::EmscriptenInputSystem()
{
	// emscripten_set_keydown_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::KeyDownCallback);
	// emscripten_set_keyup_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::KeyUpCallback);
	emscripten_set_mousemove_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseCallback);
	emscripten_set_mousedown_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseDownCallback);
	emscripten_set_mouseup_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseUpCallback);
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
		if (event.type == SDL_EventType::SDL_KEYDOWN || event.type == SDL_EventType::SDL_KEYUP)
		{
			KeyboardCode keyCode;
			switch (event.key.keysym.scancode)
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
			case SDL_SCANCODE_DELETE:
				keyCode = KeyboardCode::Delete;
				break;
			case SDL_SCANCODE_LALT:
				keyCode = KeyboardCode::LAlt;
				break;
			case SDL_SCANCODE_RALT:
				keyCode = KeyboardCode::RAlt;
				break;
			case SDL_SCANCODE_LSHIFT:
				keyCode = KeyboardCode::LShift;
				break;
			case SDL_SCANCODE_RSHIFT:
				keyCode = KeyboardCode::RShift;
				break;
			case SDL_SCANCODE_ESCAPE:
				keyCode = KeyboardCode::Esc;
				break;
			default:
				keyCode = KeyboardCode::N;
			}
			current.keys[(int)keyCode] = event.type == SDL_KEYDOWN;
			current.keys[(int)KeyboardCode::LCtrl] = current.keys[(int)KeyboardCode::RCtrl] = SDL_GetModState() & KMOD_CTRL;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == 1)
			{
				current.mouseButtons[(int)MouseButton::Left] = true;
			}
			else if (event.button.button == 3)
			{
				current.mouseButtons[(int)MouseButton::Right] = true;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (event.button.button == 1)
			{
				current.mouseButtons[(int)MouseButton::Left] = false;
			}
			else if (event.button.button == 3)
			{
				current.mouseButtons[(int)MouseButton::Right] = false;
			}
		}
		glm::ivec2 mousePos = {};
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		current.cursorPos = mousePos;
    }
	 int mx, my;
	 SDL_GetMouseState(&mx, &my);
	 current.cursorPos.x = (float)mx;
	 current.cursorPos.y = (float)my;

}
bool EmscriptenInputSystem::IsKeyPressed(KeyboardCode kc)
{
	return current.keys[(int)kc];
}

bool EmscriptenInputSystem::WasKeyPressed(KeyboardCode kc)
{
	return previous.keys[(int)kc];
}

bool EmscriptenInputSystem::IsMouseButtonPressed(MouseButton mb)
{
	return current.mouseButtons[(int)mb];
}

bool EmscriptenInputSystem::WasMouseButtonPressed(MouseButton mb)
{
	return previous.mouseButtons[(int)mb];
}

bool EmscriptenInputSystem::WasMouseButtonClicked(MouseButton mb)
{
	return previous.mouseButtons[(int)MouseButton::Left] && !current.mouseButtons[(int)MouseButton::Left];
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
	glm::vec2 returnValue = deltaCursorPos;
	deltaCursorPos = glm::vec2(0, 0);
	return returnValue;
	
}
EM_BOOL EmscriptenInputSystem::MouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	// previous = current;
	// current.cursorPos = glm::vec2(mouseEvent->targetX, mouseEvent->targetY);

	deltaCursorPos = current.cursorPos - previous.cursorPos;
	return false;
}
EM_BOOL EmscriptenInputSystem::MouseDownCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	if ((mouseEvent->buttons & 2) == 2)
	{
		emscripten_request_pointerlock("canvas.emscripten", true);
	}
	return false;
}
EM_BOOL EmscriptenInputSystem::MouseUpCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	if ((mouseEvent->buttons & 2) != 2)
	{
		emscripten_exit_pointerlock();
		deltaCursorPos = glm::vec2(0);
	}
	return false;
}
EM_BOOL EmscriptenInputSystem::KeyDownCallback(int eventType, const EmscriptenKeyboardEvent* kbEvent, void* userData)
{
	current.keys[(int)KeyboardCode::A] = strcmp(kbEvent->key, "a") == 0;
	current.keys[(int)KeyboardCode::B] = strcmp(kbEvent->key, "b") == 0;
	current.keys[(int)KeyboardCode::C] = strcmp(kbEvent->key, "c") == 0;
	current.keys[(int)KeyboardCode::D] = strcmp(kbEvent->key, "d") == 0;
	current.keys[(int)KeyboardCode::E] = strcmp(kbEvent->key, "e") == 0;
	current.keys[(int)KeyboardCode::F] = strcmp(kbEvent->key, "f") == 0;
	current.keys[(int)KeyboardCode::G] = strcmp(kbEvent->key, "g") == 0;
	current.keys[(int)KeyboardCode::H] = strcmp(kbEvent->key, "h") == 0;
	current.keys[(int)KeyboardCode::I] = strcmp(kbEvent->key, "i") == 0;
	current.keys[(int)KeyboardCode::J] = strcmp(kbEvent->key, "j") == 0;
	current.keys[(int)KeyboardCode::K] = strcmp(kbEvent->key, "k") == 0;
	current.keys[(int)KeyboardCode::L] = strcmp(kbEvent->key, "l") == 0;
	current.keys[(int)KeyboardCode::M] = strcmp(kbEvent->key, "m") == 0;
	current.keys[(int)KeyboardCode::N] = strcmp(kbEvent->key, "n") == 0;
	current.keys[(int)KeyboardCode::O] = strcmp(kbEvent->key, "o") == 0;
	current.keys[(int)KeyboardCode::P] = strcmp(kbEvent->key, "p") == 0;
	current.keys[(int)KeyboardCode::Q] = strcmp(kbEvent->key, "q") == 0;
	current.keys[(int)KeyboardCode::R] = strcmp(kbEvent->key, "r") == 0;	
	current.keys[(int)KeyboardCode::S] = strcmp(kbEvent->key, "s") == 0;
	current.keys[(int)KeyboardCode::T] = strcmp(kbEvent->key, "t") == 0;
	current.keys[(int)KeyboardCode::U] = strcmp(kbEvent->key, "u") == 0;
	current.keys[(int)KeyboardCode::V] = strcmp(kbEvent->key, "v") == 0;
	current.keys[(int)KeyboardCode::W] = strcmp(kbEvent->key, "w") == 0;
	current.keys[(int)KeyboardCode::X] = strcmp(kbEvent->key, "x") == 0;
	current.keys[(int)KeyboardCode::Y] = strcmp(kbEvent->key, "y") == 0;	
	current.keys[(int)KeyboardCode::Z] = strcmp(kbEvent->key, "z") == 0;
	current.keys[(int)KeyboardCode::Num0] = strcmp(kbEvent->key, "0") == 0;
	current.keys[(int)KeyboardCode::Num1] = strcmp(kbEvent->key, "1") == 0;
	current.keys[(int)KeyboardCode::Num2] = strcmp(kbEvent->key, "2") == 0;
	current.keys[(int)KeyboardCode::Num3] = strcmp(kbEvent->key, "3") == 0;
	current.keys[(int)KeyboardCode::Num4] = strcmp(kbEvent->key, "4") == 0;
	current.keys[(int)KeyboardCode::Num5] = strcmp(kbEvent->key, "5") == 0;
	current.keys[(int)KeyboardCode::Num6] = strcmp(kbEvent->key, "6") == 0;
	current.keys[(int)KeyboardCode::Num7] = strcmp(kbEvent->key, "7") == 0;
	current.keys[(int)KeyboardCode::Num8] = strcmp(kbEvent->key, "8") == 0;
	current.keys[(int)KeyboardCode::Num9] = strcmp(kbEvent->key, "9") == 0;
	current.keys[(int)KeyboardCode::LCtrl] = strcmp(kbEvent->key, "Control") == 0;
	current.keys[(int)KeyboardCode::RCtrl] = strcmp(kbEvent->key, "Control") == 0;
	current.keys[(int)KeyboardCode::LShift] = strcmp(kbEvent->key, "Shift") == 0;
	current.keys[(int)KeyboardCode::RShift] = strcmp(kbEvent->key, "Shift") == 0;
	current.keys[(int)KeyboardCode::ForwardSlash] = strcmp(kbEvent->key, "/") == 0;
	current.keys[(int)KeyboardCode::BackSlash] = strcmp(kbEvent->key, "\\") == 0;
	current.keys[(int)KeyboardCode::Enter] = strcmp(kbEvent->key, "Enter") == 0;
	current.keys[(int)KeyboardCode::Esc] = strcmp(kbEvent->key, "Escape") == 0;
	return false;
}

EM_BOOL EmscriptenInputSystem::KeyUpCallback(int eventType, const EmscriptenKeyboardEvent* kbEvent, void* userData)
{
	current.keys[(int)KeyboardCode::A] = (current.keys[(int)KeyboardCode::A] && strcmp(kbEvent->key, "a") != 0);
	current.keys[(int)KeyboardCode::B] = (current.keys[(int)KeyboardCode::B] && strcmp(kbEvent->key, "b") != 0);
	current.keys[(int)KeyboardCode::C] = (current.keys[(int)KeyboardCode::C] && strcmp(kbEvent->key, "c") != 0);
	current.keys[(int)KeyboardCode::D] = (current.keys[(int)KeyboardCode::D] && strcmp(kbEvent->key, "d") != 0);
	current.keys[(int)KeyboardCode::E] = (current.keys[(int)KeyboardCode::E] && strcmp(kbEvent->key, "e") != 0);
	current.keys[(int)KeyboardCode::F] = (current.keys[(int)KeyboardCode::F] && strcmp(kbEvent->key, "f") != 0);
	current.keys[(int)KeyboardCode::G] = (current.keys[(int)KeyboardCode::G] && strcmp(kbEvent->key, "g") != 0);
	current.keys[(int)KeyboardCode::H] = (current.keys[(int)KeyboardCode::H] && strcmp(kbEvent->key, "h") != 0);
	current.keys[(int)KeyboardCode::I] = (current.keys[(int)KeyboardCode::I] && strcmp(kbEvent->key, "i") != 0);
	current.keys[(int)KeyboardCode::J] = (current.keys[(int)KeyboardCode::J] && strcmp(kbEvent->key, "j") != 0);
	current.keys[(int)KeyboardCode::K] = (current.keys[(int)KeyboardCode::K] && strcmp(kbEvent->key, "k") != 0);
	current.keys[(int)KeyboardCode::L] = (current.keys[(int)KeyboardCode::L] && strcmp(kbEvent->key, "l") != 0);
	current.keys[(int)KeyboardCode::M] = (current.keys[(int)KeyboardCode::M] && strcmp(kbEvent->key, "m") != 0);
	current.keys[(int)KeyboardCode::N] = (current.keys[(int)KeyboardCode::N] && strcmp(kbEvent->key, "n") != 0);
	current.keys[(int)KeyboardCode::O] = (current.keys[(int)KeyboardCode::O] && strcmp(kbEvent->key, "o") != 0);
	current.keys[(int)KeyboardCode::P] = (current.keys[(int)KeyboardCode::P] && strcmp(kbEvent->key, "p") != 0);
	current.keys[(int)KeyboardCode::Q] = (current.keys[(int)KeyboardCode::Q] && strcmp(kbEvent->key, "q") != 0);
	current.keys[(int)KeyboardCode::R] = (current.keys[(int)KeyboardCode::R] && strcmp(kbEvent->key, "r") != 0);	
	current.keys[(int)KeyboardCode::S] = (current.keys[(int)KeyboardCode::S] && strcmp(kbEvent->key, "s") != 0);
	current.keys[(int)KeyboardCode::T] = (current.keys[(int)KeyboardCode::T] && strcmp(kbEvent->key, "t") != 0);
	current.keys[(int)KeyboardCode::U] = (current.keys[(int)KeyboardCode::U] && strcmp(kbEvent->key, "u") != 0);
	current.keys[(int)KeyboardCode::V] = (current.keys[(int)KeyboardCode::V] && strcmp(kbEvent->key, "v") != 0);
	current.keys[(int)KeyboardCode::W] = (current.keys[(int)KeyboardCode::W] && strcmp(kbEvent->key, "w") != 0);
	current.keys[(int)KeyboardCode::X] = (current.keys[(int)KeyboardCode::X] && strcmp(kbEvent->key, "x") != 0);
	current.keys[(int)KeyboardCode::Y] = (current.keys[(int)KeyboardCode::Y] && strcmp(kbEvent->key, "y") != 0);	
	current.keys[(int)KeyboardCode::Z] = (current.keys[(int)KeyboardCode::Z] && strcmp(kbEvent->key, "z") != 0);
	current.keys[(int)KeyboardCode::Num0] = (current.keys[(int)KeyboardCode::Num0] && strcmp(kbEvent->key, "0") != 0);
	current.keys[(int)KeyboardCode::Num1] = (current.keys[(int)KeyboardCode::Num1] && strcmp(kbEvent->key, "1") != 0);
	current.keys[(int)KeyboardCode::Num2] = (current.keys[(int)KeyboardCode::Num2] && strcmp(kbEvent->key, "2") != 0);
	current.keys[(int)KeyboardCode::Num3] = (current.keys[(int)KeyboardCode::Num3] && strcmp(kbEvent->key, "3") != 0);
	current.keys[(int)KeyboardCode::Num4] = (current.keys[(int)KeyboardCode::Num4] && strcmp(kbEvent->key, "4") != 0);
	current.keys[(int)KeyboardCode::Num5] = (current.keys[(int)KeyboardCode::Num5] && strcmp(kbEvent->key, "5") != 0);
	current.keys[(int)KeyboardCode::Num6] = (current.keys[(int)KeyboardCode::Num6] && strcmp(kbEvent->key, "6") != 0);
	current.keys[(int)KeyboardCode::Num7] = (current.keys[(int)KeyboardCode::Num7] && strcmp(kbEvent->key, "7") != 0);
	current.keys[(int)KeyboardCode::Num8] = (current.keys[(int)KeyboardCode::Num8] && strcmp(kbEvent->key, "8") != 0);
	current.keys[(int)KeyboardCode::Num9] = (current.keys[(int)KeyboardCode::Num9] && strcmp(kbEvent->key, "9") != 0);
	current.keys[(int)KeyboardCode::LCtrl] = (current.keys[(int)KeyboardCode::LCtrl] && strcmp(kbEvent->key, "Control") != 0);
	current.keys[(int)KeyboardCode::RCtrl] = (current.keys[(int)KeyboardCode::RCtrl] && strcmp(kbEvent->key, "Control") != 0);
	current.keys[(int)KeyboardCode::LShift] = (current.keys[(int)KeyboardCode::LShift] && strcmp(kbEvent->key, "Shift") != 0);
	current.keys[(int)KeyboardCode::RShift] = (current.keys[(int)KeyboardCode::RShift] && strcmp(kbEvent->key, "Shift") != 0);
	current.keys[(int)KeyboardCode::ForwardSlash] = (current.keys[(int)KeyboardCode::ForwardSlash] && strcmp(kbEvent->key, "/") != 0);
	current.keys[(int)KeyboardCode::BackSlash] = (current.keys[(int)KeyboardCode::BackSlash] && strcmp(kbEvent->key, "\\") != 0);
	current.keys[(int)KeyboardCode::Enter] = (current.keys[(int)KeyboardCode::Enter] && strcmp(kbEvent->key, "Enter") != 0);
	current.keys[(int)KeyboardCode::Esc] = (current.keys[(int)KeyboardCode::Esc] && strcmp(kbEvent->key, "Escape") != 0);
	return false;
}
#endif