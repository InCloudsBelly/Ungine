#pragma once

namespace U
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

#define U_KEY_SPACE           ::U::Key::Space
#define U_KEY_APOSTROPHE      ::U::Key::Apostrophe    /* ' */
#define U_KEY_COMMA           ::U::Key::Comma         /* , */
#define U_KEY_MINUS           ::U::Key::Minus         /* - */
#define U_KEY_PERIOD          ::U::Key::Period        /* . */
#define U_KEY_SLASH           ::U::Key::Slash         /* / */
#define U_KEY_0               ::U::Key::D0
#define U_KEY_1               ::U::Key::D1
#define U_KEY_2               ::U::Key::D2
#define U_KEY_3               ::U::Key::D3
#define U_KEY_4               ::U::Key::D4
#define U_KEY_5               ::U::Key::D5
#define U_KEY_6               ::U::Key::D6
#define U_KEY_7               ::U::Key::D7
#define U_KEY_8               ::U::Key::D8
#define U_KEY_9               ::U::Key::D9
#define U_KEY_SEMICOLON       ::U::Key::Semicolon     /* ; */
#define U_KEY_EQUAL           ::U::Key::Equal         /* = */
#define U_KEY_A               ::U::Key::A
#define U_KEY_B               ::U::Key::B
#define U_KEY_C               ::U::Key::C
#define U_KEY_D               ::U::Key::D
#define U_KEY_E               ::U::Key::E
#define U_KEY_F               ::U::Key::F
#define U_KEY_G               ::U::Key::G
#define U_KEY_H               ::U::Key::H
#define U_KEY_I               ::U::Key::I
#define U_KEY_J               ::U::Key::J
#define U_KEY_K               ::U::Key::K
#define U_KEY_L               ::U::Key::L
#define U_KEY_M               ::U::Key::M
#define U_KEY_N               ::U::Key::N
#define U_KEY_O               ::U::Key::O
#define U_KEY_P               ::U::Key::P
#define U_KEY_Q               ::U::Key::Q
#define U_KEY_R               ::U::Key::R
#define U_KEY_S               ::U::Key::S
#define U_KEY_T               ::U::Key::T
#define U_KEY_U               ::U::Key::U
#define U_KEY_V               ::U::Key::V
#define U_KEY_W               ::U::Key::W
#define U_KEY_X               ::U::Key::X
#define U_KEY_Y               ::U::Key::Y
#define U_KEY_Z               ::U::Key::Z
#define U_KEY_LEFT_BRACKET    ::U::Key::LeftBracket   /* [ */
#define U_KEY_BACKSLASH       ::U::Key::Backslash     /* \ */
#define U_KEY_RIGHT_BRACKET   ::U::Key::RightBracket  /* ] */
#define U_KEY_GRAVE_ACCENT    ::U::Key::GraveAccent   /* ` */
#define U_KEY_WORLD_1         ::U::Key::World1        /* non-US #1 */
#define U_KEY_WORLD_2         ::U::Key::World2        /* non-US #2 */


/* Function keys */
#define U_KEY_ESCAPE          ::U::Key::Escape
#define U_KEY_ENTER           ::U::Key::Enter
#define U_KEY_TAB             ::U::Key::Tab
#define U_KEY_BACKSPACE       ::U::Key::Backspace
#define U_KEY_INSERT          ::U::Key::Insert
#define U_KEY_DELETE          ::U::Key::Delete
#define U_KEY_RIGHT           ::U::Key::Right
#define U_KEY_LEFT            ::U::Key::Left
#define U_KEY_DOWN            ::U::Key::Down
#define U_KEY_UP              ::U::Key::Up
#define U_KEY_PAGE_UP         ::U::Key::PageUp
#define U_KEY_PAGE_DOWN       ::U::Key::PageDown
#define U_KEY_HOME            ::U::Key::Home
#define U_KEY_END             ::U::Key::End
#define U_KEY_CAPS_LOCK       ::U::Key::CapsLock
#define U_KEY_SCROLL_LOCK     ::U::Key::ScrollLock
#define U_KEY_NUM_LOCK        ::U::Key::NumLock
#define U_KEY_PRINT_SCREEN    ::U::Key::PrintScreen
#define U_KEY_PAUSE           ::U::Key::Pause
#define U_KEY_F1              ::U::Key::F1
#define U_KEY_F2              ::U::Key::F2
#define U_KEY_F3              ::U::Key::F3
#define U_KEY_F4              ::U::Key::F4
#define U_KEY_F5              ::U::Key::F5
#define U_KEY_F6              ::U::Key::F6
#define U_KEY_F7              ::U::Key::F7
#define U_KEY_F8              ::U::Key::F8
#define U_KEY_F9              ::U::Key::F9
#define U_KEY_F10             ::U::Key::F10
#define U_KEY_F11             ::U::Key::F11
#define U_KEY_F12             ::U::Key::F12
#define U_KEY_F13             ::U::Key::F13
#define U_KEY_F14             ::U::Key::F14
#define U_KEY_F15             ::U::Key::F15
#define U_KEY_F16             ::U::Key::F16
#define U_KEY_F17             ::U::Key::F17
#define U_KEY_F18             ::U::Key::F18
#define U_KEY_F19             ::U::Key::F19
#define U_KEY_F20             ::U::Key::F20
#define U_KEY_F21             ::U::Key::F21
#define U_KEY_F22             ::U::Key::F22
#define U_KEY_F23             ::U::Key::F23
#define U_KEY_F24             ::U::Key::F24
#define U_KEY_F25             ::U::Key::F25

/* Keypad */
#define U_KEY_KP_0            ::U::Key::KP0
#define U_KEY_KP_1            ::U::Key::KP1
#define U_KEY_KP_2            ::U::Key::KP2
#define U_KEY_KP_3            ::U::Key::KP3
#define U_KEY_KP_4            ::U::Key::KP4
#define U_KEY_KP_5            ::U::Key::KP5
#define U_KEY_KP_6            ::U::Key::KP6
#define U_KEY_KP_7            ::U::Key::KP7
#define U_KEY_KP_8            ::U::Key::KP8
#define U_KEY_KP_9            ::U::Key::KP9
#define U_KEY_KP_DECIMAL      ::U::Key::KPDecimal
#define U_KEY_KP_DIVIDE       ::U::Key::KPDivide
#define U_KEY_KP_MULTIPLY     ::U::Key::KPMultiply
#define U_KEY_KP_SUBTRACT     ::U::Key::KPSubtract
#define U_KEY_KP_ADD          ::U::Key::KPAdd
#define U_KEY_KP_ENTER        ::U::Key::KPEnter
#define U_KEY_KP_EQUAL        ::U::Key::KPEqual

#define U_KEY_LEFT_SHIFT      ::U::Key::LeftShift
#define U_KEY_LEFT_CONTROL    ::U::Key::LeftControl
#define U_KEY_LEFT_ALT        ::U::Key::LeftAlt
#define U_KEY_LEFT_SUPER      ::U::Key::LeftSuper
#define U_KEY_RIGHT_SHIFT     ::U::Key::RightShift
#define U_KEY_RIGHT_CONTROL   ::U::Key::RightControl
#define U_KEY_RIGHT_ALT       ::U::Key::RightAlt
#define U_KEY_RIGHT_SUPER     ::U::Key::RightSuper
#define U_KEY_MENU            ::U::Key::Menu


#define U_MOUSE_BUTTON_LEFT    0
#define U_MOUSE_BUTTON_RIGHT   1
#define U_MOUSE_BUTTON_MIDDLE  2
