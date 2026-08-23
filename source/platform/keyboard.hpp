#pragma once

#include <core/types.hpp>
#include <pipeline.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

enum
{
#if PLATFORM_LINUX
	VK_Alt = 0xE9,
	VK_Alt_r = 0xEA,
	VK_Aab = 0x09,
	VK_Pageup = 0x55,
	VK_Pagedown = 0x56,
	VK_End = 0x57,
	VK_Home = 0x50,
	VK_Delete = 0xFF,
	VK_Enter = 0x0D,
	VK_Backspace = 0x08,
	VK_Shift = 0xE1,
	VK_Shift_r = 0xE2,
	VK_Control = 0xE3,
	VK_Control_r = 0xE4,
	VK_Command = VK_Control,
	VK_Escape = 0x1B,
	VK_Space = 0x20,
	VK_Equal = 0x3D,
	VK_Minus = 0x2D,
	VK_Up = 0x52,
	VK_Right = 0x53,
	VK_Down = 0x54,
	VK_Left = 0x51,
	VK_F1 = 0xBE,
	VK_F2 = 0xBF,
	VK_F3 = 0xC0,
	VK_F4 = 0xC1,
	VK_F5 = 0xC2,
	VK_F6 = 0xC3,
	VK_F7 = 0xC4,
	VK_F8 = 0xC5,
	VK_F9 = 0xC6,
	VK_F10 = 0xC7,
	VK_F11 = 0xC8,
	VK_F12 = 0xC9,
	VK_Tilde = 0x60,
	VK_Keypad0 = 0x9E,
	VK_Keypad1 = 0x9C,
	VK_Keypad2 = 0x99,
	VK_Keypad3 = 0x9B,
	VK_Keypad4 = 0x96,
	VK_Keypad5 = 0x9D,
	VK_Keypad6 = 0x98,
	VK_Keypad7 = 0x95,
	VK_Keypad8 = 0x97,
	VK_Keypad9 = 0x9A,
	VK_A = 'a',
	VK_B = 'b',
	VK_C = 'c',
	VK_D = 'd',
	VK_E = 'e',
	VK_F = 'f',
	VK_G = 'g',
	VK_H = 'h',
	VK_I = 'i',
	VK_J = 'j',
	VK_K = 'k',
	VK_L = 'l',
	VK_M = 'm',
	VK_N = 'n',
	VK_O = 'o',
	VK_P = 'p',
	VK_Q = 'q',
	VK_R = 'r',
	VK_S = 's',
	VK_T = 't',
	VK_U = 'u',
	VK_V = 'v',
	VK_W = 'w',
	VK_X = 'x',
	VK_Y = 'y',
	VK_Z = 'z',
	VK_0 = '0',
	VK_1 = '1',
	VK_2 = '2',
	VK_3 = '3',
	VK_4 = '4',
	VK_5 = '5',
	VK_6 = '6',
	VK_7 = '7',
	VK_8 = '8',
	VK_9 = '9',
#elif PLATFORM_WINDOWS

#endif
};

class Keyboard
{
private:
	static Keyboard *keyboard;

public:
	Keyboard()
	{
		keyboard_clear();
	}

	struct State
	{
		bool keyCurrent[256];
		bool keyPrevious[256];
		bool keyRepeat[256];
		float keyRepeatTimer[256];
		char inputBuffer[16];
	};

	static State &state();

	static void reset_active();
	static void set_active(Keyboard &keyboard);

	static bool check(uint_8 key) { return keyboard->keyboard_check(key); }
	static bool check_pressed(uint_8 key) { return keyboard->keyboard_check_pressed(key); }
	static bool check_pressed_repeat(uint_8 key) { return keyboard->keyboard_check_pressed_repeat(key); }
	static bool check_released(uint_8 key) { return keyboard->keyboard_check_released(key); }
	static bool check_any() { return keyboard->keyboard_check_any(); }
	static bool check_pressed_any() { return keyboard->keyboard_check_pressed_any(); }
	static bool check_pressed_repeat_any() { return keyboard->keyboard_check_pressed_repeat_any(); }
	static bool check_released_any() { return keyboard->keyboard_check_released_any(); }

	static bool has_input() { return keyboard->keyboard_has_input(); }
	static char *input_buffer() { return keyboard->keyboard_input_buffer(); }

	static void update(uint_64 delta) { return keyboard->keyboard_update(delta); }
	static void clear() { return keyboard->keyboard_clear(); }

private:
	bool keyboard_check(uint_8 key);
	bool keyboard_check_pressed(uint_8 key);
	bool keyboard_check_pressed_repeat(uint_8 key);
	bool keyboard_check_released(uint_8 key);

	bool keyboard_check_any();
	bool keyboard_check_pressed_any();
	bool keyboard_check_pressed_repeat_any();
	bool keyboard_check_released_any();

	bool keyboard_has_input();
	char *keyboard_input_buffer();

	void keyboard_update(uint_64 delta);
	void keyboard_clear();

public:
	State keyboardState;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////