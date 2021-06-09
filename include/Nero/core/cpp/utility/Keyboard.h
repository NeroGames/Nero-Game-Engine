////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef KEYBOARD_H
#define KEYBOARD_H
///////////////////////////HEADERS//////////////////////////
////////////////////////////////////////////////////////////
namespace nero
{
	namespace keyboard
	{
		bool	CTRL();
		bool	SHIFT();
		bool	ALT();
		bool	CTRL_SHIFT();
		bool	CTRL_ALT();
		bool	SHIFT_ALT();
		bool	CTRL_SHIFT_ALT();
	}

	struct ModifierKey
	{
		bool alt        = false;
		bool control    = false;
		bool shift      = false;
		bool system     = false;
	};
}
#endif // KEYBOARD_H
