#ifndef XBOXCONTROLLER_H_
#define XBOXCONTROLLER_H_

#include <Object.h>
#include <Windows.h>
#include <Mmsystem.h>

#include <iostream>

class XBOXController : public yam2d::Object
{
	UINT joyID;

	static float _c1X,_c1Y,_c2X,_c2Y;
	static float _errVal;

public:
	enum Axises
	{
		XAxis = 0,
		YAxis = 1,
		ZAxis = 2,
		RAxis = 3,
		UAxis = 4,
		VAxis = 5
	};

	enum Buttons
	{
		Button_A = 0x00001,
		Button_B = 0x00002,
		Button_X = 0x00004,
		Button_Y = 0x00008,
		Button_Back = 0x00040,
		Button_Start = 0x00080,
		Button_LSB = 0x00100,
		Button_RSB = 0x00200
	};

	enum DPad
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
	bool isConnected();
	bool isButtonDown(int button);
	bool isDPadPointing(int val);
	float getAxis(int i);
	float updateAxis(int i);
	static void manualUpdateValues(float c1X, float c1Y, float c2X, float c2Y);
	XBOXController(UINT ID);
	virtual ~XBOXController(void);



};

#endif