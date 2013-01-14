// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// http://code.google.com/p/yam2d/
//
// Copyright (c) 2013 Mikko Romppainen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef INPUT_H_
#define INPUT_H_

enum KeyCodes
{
	KEY_BACK		= 0x08,
	KEY_TAB			= 0x09,
	KEY_CLEAR		= 0x0C,
	KEY_RETURN		= 0x0D,
	KEY_SHIFT		= 0x10,
	KEY_CONTROL		= 0x11,
	KEY_MENU		= 0x12,
	KEY_PAUSE		= 0x13,
	KEY_CAPITAL		= 0x14,
	KEY_ESCAPE		= 0x1B,
	KEY_SPACE		= 0x20,
	KEY_PRIOR		= 0x21,
	KEY_NEXT		= 0x22,
	KEY_END			= 0x23,
	KEY_HOME		= 0x24,
	KEY_LEFT		= 0x25,
	KEY_UP			= 0x26,
	KEY_RIGHT		= 0x27,
	KEY_DOWN		= 0x28,
	KEY_SELECT		= 0x29,
	KEY_PRINT		= 0x2A,
	KEY_SNAPSHOT	= 0x2C,
	KEY_INSER		= 0x2D,
	KEY_DELETE		= 0x2E,
	KEY_HELP		= 0x2F,
	KEY_0			= '0',
	KEY_1			= '1',
	KEY_2			= '2',
	KEY_3			= '3',
	KEY_4			= '4',
	KEY_5			= '5',
	KEY_6			= '6',
	KEY_7			= '7',
	KEY_8			= '8',
	KEY_9			= '9',
	KEY_A			= 'A',
	KEY_B			= 'B',
	KEY_C			= 'C',
	KEY_D			= 'D',
	KEY_E			= 'E',
	KEY_F			= 'F',
	KEY_G			= 'G',
	KEY_H			= 'H',
	KEY_I			= 'I',
	KEY_J			= 'J',
	KEY_K			= 'K',
	KEY_L			= 'L',
	KEY_M			= 'M',
	KEY_N			= 'N',
	KEY_O			= 'O',
	KEY_P			= 'P',
	KEY_Q			= 'Q',
	KEY_R			= 'R',
	KEY_S			= 'S',
	KEY_T			= 'T',
	KEY_U			= 'U',
	KEY_V			= 'V',
	KEY_W			= 'W',
	KEY_X			= 'X',
	KEY_Y			= 'Y',
	KEY_Z			= 'Z',
	KEY_NUMPAD0		= 0x60,
	KEY_NUMPAD1		= 0x61,
	KEY_NUMPAD2		= 0x62,
	KEY_NUMPAD3		= 0x63,
	KEY_NUMPAD4		= 0x64,
	KEY_NUMPAD5		= 0x65,
	KEY_NUMPAD6		= 0x66,
	KEY_NUMPAD7		= 0x67,
	KEY_NUMPAD8		= 0x68,
	KEY_NUMPAD9		= 0x69,
	KEY_MULTIPLY	= 0x6A,
	KEY_ADD			= 0x6B,
	KEY_SEPARATOR	= 0x6C,
	KEY_SUBTRACT	= 0x6D,
	KEY_DECIMAL		= 0x6E,
	KEY_DIVIDE		= 0x6F,
	KEY_F1			= 0x70,
	KEY_F2			= 0x71,
	KEY_F3			= 0x72,
	KEY_F4			= 0x73,
	KEY_F5			= 0x74,
	KEY_F6			= 0x75,
	KEY_F7			= 0x76,
	KEY_F8			= 0x77,
	KEY_F9			= 0x78,
	KEY_F10			= 0x79,
	KEY_F11			= 0x7A,
	KEY_F12			= 0x7B,
	KEY_F13			= 0x7C,
	KEY_F14			= 0x7D,
	KEY_F15			= 0x7E,
	KEY_F16			= 0x7F,
	KEY_F17			= 0x80,
	KEY_F18			= 0x81,
	KEY_F19			= 0x82,
	KEY_F20			= 0x83,
	KEY_F21			= 0x84,
	KEY_F22			= 0x85,
	KEY_F23			= 0x86,
	KEY_F24			= 0x87
};

/**
 * Returns key button state. Returns 0 if button is not pressed returns 1. if button is pressed down.
 */
int getKeyState(KeyCodes keyCode);


#endif


