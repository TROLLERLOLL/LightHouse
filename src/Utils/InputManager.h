#pragma once
#include <unordered_map>

enum class KeyCodes {
	NONE,
	ESCAPE,

	CTRL,
	LCTRL,
	RCTRL,

	ALT,
	LALT,
	RALT,

	SHIFT,
	LSHIFT,
	RSHIFT,

	SPACE,
	TAB,
	BACK,
	ENTER,

	UP, DOWN,
	LEFT, RIGHT,

	A, B, C, D, E, F,
	G, H, I, J, K, L,
	M, N, O, P, Q, R,
	S, T, U, V, W, X,
	Y, Z,

	NUM_0, NUM_1, 
	NUM_2, NUM_3, 
	NUM_4, NUM_5,
	NUM_6, NUM_7,
	NUM_8, NUM_9,

	F1, F2, F3, F4,
	F5, F6, F7, F8,
	F9, F10, F11, F12,

	LastKey
};

class InputManager {
	private:
		std::unordered_map<KeyCodes, bool> current;
		std::unordered_map<KeyCodes, bool> previous;
	public:
		InputManager() {
			for (int i = (int)KeyCodes::NONE; i <= (int)KeyCodes::LastKey; i++) {
				previous.emplace((KeyCodes)i, false);
				current.emplace((KeyCodes)i, false);
			}
		}

		inline void setKey(KeyCodes keycode, bool down) {
			if (keycode == KeyCodes::NONE || keycode == KeyCodes::LastKey || keycode == KeyCodes::CTRL || keycode == KeyCodes::ALT || keycode == KeyCodes::SHIFT)
				return;

			current[keycode] = down;

			if (keycode == KeyCodes::LALT || keycode == KeyCodes::RALT)
				current[KeyCodes::ALT] = current[KeyCodes::LALT] || current[KeyCodes::RALT];
			else if (keycode == KeyCodes::LCTRL || keycode == KeyCodes::RCTRL)
				current[KeyCodes::CTRL] = current[KeyCodes::LCTRL] || current[KeyCodes::RCTRL];
			else if (keycode == KeyCodes::LSHIFT || keycode == KeyCodes::RSHIFT)
				current[KeyCodes::SHIFT] = current[KeyCodes::LSHIFT] || current[KeyCodes::RSHIFT];
		}
		inline void Update() {
			for (auto key : current) {
				previous[key.first] = key.second == true;
			}
		}

		inline bool isKeyDown(KeyCodes keycode) { 
			return current[keycode];
		}
		inline bool isKeyJustDown(KeyCodes keycode) { 
			return isKeyDown(keycode) && wasKeyUp(keycode); 
		}
		inline bool isKeyUp(KeyCodes keycode) { return !isKeyDown(keycode); }
		inline bool isKeyJustUp(KeyCodes keycode) { return wasKeyDown(keycode) && isKeyUp(keycode); }
		
		inline bool wasKeyDown(KeyCodes keycode) { 
			return previous[keycode]; 
		}
		inline bool wasKeyUp(KeyCodes keycode) { 
			return !wasKeyDown(keycode); 
		}
};