#ifndef XBOXCONTROLLER_H_
#define XBOXCONTROLLER_H_

#include <Object.h>
#include <Windows.h>
#include <Mmsystem.h>

#include <iostream>

class XBOXController : public yam2d::Object
{
public:
	enum JoyAxis
	{
		XAxis = 0,
		YAxis = 1,
		ZAxis = 2,
		RAxis = 3,
		UAxis = 4,
		VAxis = 5
	};

	enum JoyButtons
	{
		Button_A = 1 << 0,
		Button_B = 1 << 1,
		Button_X = 1 << 2,
		Button_Y = 1 << 3,
		Button_LSB = 1 << 4,
		Button_RSB = 1 << 5,
		Button_Back = 1 << 6,
		Button_Start = 1 << 7
	};

	enum DPadDir
	{
		DPad_Up = 0,
		DPad_UpRight = 4500,
		DPad_Right = 9000,
		DPad_DownRight = 13500,
		DPad_Down = 18000,
		DPad_DownLeft = 22500,
		DPad_Left = 27000,
		DPad_UpLeft = 31500
	};

	XBOXController(int controllerIndex);
	virtual ~XBOXController(void);

	bool isConnected();

	bool isButtonDown(JoyButtons button);
	bool isButtonPressed(JoyButtons button);
	bool isButtonReleased(JoyButtons button);

	bool isDPadPointing(DPadDir val);
	bool isDPadPressed();
	float getDPadAngle();
	float getDPadNewAngle();

	float getAxis(JoyAxis i);

	void updateAllAxis();

	int getId() { return (int)m_joyID; }
private:
	const UINT m_joyID;
	bool m_connected;
	JOYCAPS m_caps;
	JOYINFOEX joyInfoEx;
	JOYINFOEX m_prevJoyInfoEx;
};

#endif