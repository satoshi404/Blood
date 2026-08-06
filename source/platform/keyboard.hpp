#pragma once

#include <core/types.hpp>
#include <pipeline.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

enum
{
#if PLATFORM_LINUX
	KeyCodeType_Alt = 0xE9,
	KeyCodeType_Alt_r = 0xEA,
	KeyCodeType_Aab = 0x09,
	KeyCodeType_Pageup = 0x55,
	KeyCodeType_Pagedown = 0x56,
	KeyCodeType_End = 0x57,
	KeyCodeType_Home = 0x50,
	KeyCodeType_Delete = 0xFF,
	KeyCodeType_Enter = 0x0D,
	KeyCodeType_Backspace = 0x08,
	KeyCodeType_Shift = 0xE1,
	KeyCodeType_Shift_r = 0xE2,
	KeyCodeType_Control = 0xE3,
	KeyCodeType_Control_r = 0xE4,
	KeyCodeType_Command = KeyCodeType_Control,
	KeyCodeType_Escape = 0x1B,
	KeyCodeType_Space = 0x20,
	KeyCodeType_Equal = 0x3D,
	KeyCodeType_Minus = 0x2D,
	KeyCodeType_Up = 0x52,
	KeyCodeType_Right = 0x53,
	KeyCodeType_Down = 0x54,
	KeyCodeType_Left = 0x51,
	KeyCodeType_F1 = 0xBE,
	KeyCodeType_F2 = 0xBF,
	KeyCodeType_F3 = 0xC0,
	KeyCodeType_F4 = 0xC1,
	KeyCodeType_F5 = 0xC2,
	KeyCodeType_F6 = 0xC3,
	KeyCodeType_F7 = 0xC4,
	KeyCodeType_F8 = 0xC5,
	KeyCodeType_F9 = 0xC6,
	KeyCodeType_F10 = 0xC7,
	KeyCodeType_F11 = 0xC8,
	KeyCodeType_F12 = 0xC9,
	KeyCodeType_Tilde = 0x60,
	KeyCodeType_Keypad0 = 0x9E,
	KeyCodeType_Keypad1 = 0x9C,
	KeyCodeType_Keypad2 = 0x99,
	KeyCodeType_Keypad3 = 0x9B,
	KeyCodeType_Keypad4 = 0x96,
	KeyCodeType_Keypad5 = 0x9D,
	KeyCodeType_Keypad6 = 0x98,
	KeyCodeType_Keypad7 = 0x95,
	KeyCodeType_Keypad8 = 0x97,
	KeyCodeType_Keypad9 = 0x9A,
	KeyCodeType_A = 'a',
	KeyCodeType_B = 'b',
	KeyCodeType_C = 'c',
	KeyCodeType_D = 'd',
	KeyCodeType_E = 'e',
	KeyCodeType_F = 'f',
	KeyCodeType_G = 'g',
	KeyCodeType_H = 'h',
	KeyCodeType_I = 'i',
	KeyCodeType_J = 'j',
	KeyCodeType_K = 'k',
	KeyCodeType_L = 'l',
	KeyCodeType_M = 'm',
	KeyCodeType_N = 'n',
	KeyCodeType_O = 'o',
	KeyCodeType_P = 'p',
	KeyCodeType_Q = 'q',
	KeyCodeType_R = 'r',
	KeyCodeType_S = 's',
	KeyCodeType_T = 't',
	KeyCodeType_U = 'u',
	KeyCodeType_V = 'v',
	KeyCodeType_W = 'w',
	KeyCodeType_X = 'x',
	KeyCodeType_Y = 'y',
	KeyCodeType_Z = 'z',
	KeyCodeType_0 = '0',
	KeyCodeType_1 = '1',
	KeyCodeType_2 = '2',
	KeyCodeType_3 = '3',
	KeyCodeType_4 = '4',
	KeyCodeType_5 = '5',
	KeyCodeType_6 = '6',
	KeyCodeType_7 = '7',
	KeyCodeType_8 = '8',
	KeyCodeType_9 = '9',
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
	static void set_active( Keyboard &keyboard );

	static bool check( u8 key ) { return keyboard->keyboard_check( key ); }
	static bool check_pressed( u8 key ) { return keyboard->keyboard_check_pressed( key ); }
	static bool check_pressed_repeat( u8 key ) { return keyboard->keyboard_check_pressed_repeat( key ); }
	static bool check_released( u8 key ) { return keyboard->keyboard_check_released( key ); }
	static bool check_any() { return keyboard->keyboard_check_any(); }
	static bool check_pressed_any() { return keyboard->keyboard_check_pressed_any(); }
	static bool check_pressed_repeat_any() { return keyboard->keyboard_check_pressed_repeat_any(); }
	static bool check_released_any() { return keyboard->keyboard_check_released_any(); }

	static bool has_input() { return keyboard->keyboard_has_input(); }
	static char *input_buffer() { return keyboard->keyboard_input_buffer(); }

	static void update( u64 delta ) { return keyboard->keyboard_update( delta ); }
	static void clear() { return keyboard->keyboard_clear(); }

private:
	bool keyboard_check( u8 key );
	bool keyboard_check_pressed( u8 key );
	bool keyboard_check_pressed_repeat( u8 key );
	bool keyboard_check_released( u8 key );

	bool keyboard_check_any();
	bool keyboard_check_pressed_any();
	bool keyboard_check_pressed_repeat_any();
	bool keyboard_check_released_any();

	bool keyboard_has_input();
	char *keyboard_input_buffer();

	void keyboard_update( u64 delta );
	void keyboard_clear();

public:
	State keyboardState;
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////