#ifndef ENGINEHELP_H_INCLUDED
#define ENGINEHELP_H_INCLUDED

#include <string>

const std::string ENGINE_HELP =
"Nero Game Engine Help v0.9.0\n\
\n\
This is a quick help, for more details information please\n\
access the learning page using the button [ Learn ] on\n\
the left\n\
\n\
# Mouse on Canvas : \n\
\n\
Certain actions are only possible when the mouse is on\n\
the canvas. Example :\n\
    - Control the camera\n\
    - Change the Engine sub mode\n\
\n\
# Control : \n\
\n\
The Engine uses the following Keys\n\
	- Modifier : CTRL, SHIFT, ALT\n\
	- Mouse (Left, Right, Middle Buttons, MouseWheel)\n\
	- Numpad (1 to 9, Multiply, Divide)\n\
	- Spacebar, Return, Delete, Arrows (Left, Up, Right), Z, Y\n\
\n\
# Camera\n\
\n\
Control the Camera with the Numpad or the Mouse\n\
    - Numpad_4 : move left\n\
    - Numpad_6 : move right\n\
    - Numpad_8 : move up\n\
    - Numpad_2 : move down\n\
    - Numpad_7 : rotate left\n\
    - Numpad_9 : rotate right\n\
    - Numpad_Multiply : zoom in\n\
    - Numpad_Divide : zoom out\n\
    - Numpad_5 : reset camera state\n\
    - ALT + Numpad_5 : save camera state\n\
\n\
    - CTRL + MouseWheel : move left/right\n\
    - ALT + MouseWheel : move up/down\n\
    - SHIFT + MouseWheel : rotate left/right\n\
    - ALT + Mouse MiddleButton : save camera state\n\
    - Mouse MiddleButton : reset camera state\n\
    - MouseWheel : zoom in/out\n\
\n\
# Engine Sub Mode\n\
\n\
The Engine has three sub modes : Object, Mesh, Play\n\
    - Object : Add, Modify, Duplicate, Delete Objects\n\
    - Mesh : Modify mesh (add vertices, extrude lines)\n\
    - Play : Test a Scene in real time\n\
\n\
Use the Spacebar to change the Engine sub mode\n\
    - Spacebar : switch between Mesh and Object modes\n\
    - CTRL + Spacebar : access Play mode\n\
\n\
# Undo or Redo\n\
Most actions on Objects can be canceled on re-applied \n\
 	- Undo : Ctrl + Z\n\
 	- Redo : Ctrl + Y\n\
\n\
# List of Object Types\n\
	- Sprite\n\
	- Mesh\n\
	- Animation\n\
	- Text\n\
	- Button\n\
    - Meshed (Sprite + Mesh)\n\
	- Meshed_Animation (Animation + Mesh)\n\
\n\
# Delete Object\n\
\n\
    - ALT + Mouse Left Button\n\
    - Delete Button\n\
\n\
# Object Control\n\
\n\
The following actions apply to any object\n\
    - CTRL + Numpad_4 : move left\n\
    - CTRL + Numpad_6 : move right\n\
    - CTRL + Numpad_8 : move up\n\
    - CTRL + Numpad_2 : move down\n\
    - CTRL + Numpad_7 : rotate left\n\
    - CTRL + Numpad_9 : rotate right\n\
    - CTRL + Numpad_Multiply : zoom in\n\
    - CTRL + Numpad_Divide : zoom out\n\
\n\
    - CTRL + Mouse Move Up/Down : zoom in/out\n\
    - SHIFT + Mouse Move Up/Down : rotate left/right\n\
\n\
The following actions apply to Objects with Sprite\n\
    - CTRL + Numpad_1 : flip horizontally\n\
    - CTRL + Numpad_3 : flip vertically\n\
\n\
The following actions apply to an entire Layer \n\
    - CTRL + ALT + Numpad_4 : move left\n\
    - CTRL + ALT + Numpad_6 : move right\n\
    - CTRL + ALT + Numpad_8 : move up\n\
    - CTRL + ALT + Numpad_2 : move down\n\
\n\
# Color Picker\n\
\n\
The color picker is hidden by default, it appears\n\
when the Color_Tab becomes active\n\
\n\
# List of Resources\n\
\n\
The Engine load all resources or startup\n\
	- Font\n\
	- Texture\n\
	- Animation\n\
	- Sound\n\
	- Music\n\
	- Script\n\
	- Shader\n\
\n\
";


#endif // ENGINEHELP_H_INCLUDED
