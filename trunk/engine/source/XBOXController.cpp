#include "../../include/XBOXController.h"
#include "vec2.h"

float XBOXController::_c1X = 0;
float XBOXController::_c1Y = 0;
float XBOXController::_c2X = 0;
float XBOXController::_c2Y = 0;
float XBOXController::_errVal = 0;


XBOXController::XBOXController(UINT ID)
{
	joyID = ID;
	_errVal = 32767;
	_errVal -= (65535.0f * 0.5f);
	_errVal /= 32767.0f;
}


bool XBOXController::isConnected()
{
	JOYINFOEX _joyInfoEx;
	if(joyGetPosEx(joyID,&_joyInfoEx) == JOYERR_NOERROR)
	{
		return true;
	}
	return false;
}


bool XBOXController::isButtonDown(int i)
{
		JOYINFOEX _joyInfoEx;
		_joyInfoEx.dwSize = sizeof(_joyInfoEx);
		_joyInfoEx.dwFlags = (JOY_RETURNALL);

		if(joyGetPosEx(joyID, &_joyInfoEx) == JOYERR_NOERROR)
		{
			if(_joyInfoEx.dwButtons & i)
			{

				return true;
			}
		}
	return false;
}


float XBOXController::getAxis(int i)
{
	float retVal = 0;
	switch(joyID)
	{
	case 0:
		if(i == XAxis)
			retVal = _c1X;
		else if(i == YAxis)
			retVal = _c1Y;
		else
			retVal = 0;
		break;
	
	case 1:
		if(i == XAxis)
			retVal = _c2X;
		else if(i == YAxis)
			retVal = _c2Y;
		else
			retVal = 0;
		break;
	}

	if(retVal < -0.4f || retVal > 0.4f)
		return retVal;
	return 0;
}


float XBOXController::updateAxis(int i)
{
	JOYINFOEX _joyInfoEx;
	_joyInfoEx.dwSize = sizeof(_joyInfoEx);
	_joyInfoEx.dwFlags = (JOY_RETURNX |JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR |JOY_RETURNU |JOY_RETURNV);
	float retVal = 0;
	joyGetPosEx(joyID, &_joyInfoEx);
	if(joyGetPosEx(joyID, &_joyInfoEx) == JOYERR_NOERROR)
	{	
		switch(i)
		{
		case XAxis:
			retVal = (float)_joyInfoEx.dwXpos;
			break;
		case YAxis:
			retVal = (float)_joyInfoEx.dwYpos;
			break;
		case ZAxis:
			retVal = (float)_joyInfoEx.dwZpos;
			break;
		case RAxis:
			retVal = (float)_joyInfoEx.dwRpos;
			break;
		case UAxis:
			retVal = (float)_joyInfoEx.dwUpos;
			break;
		case VAxis:
			retVal = (float)_joyInfoEx.dwVpos;
			break;
		default:
			retVal = -1;
			break;
		}

		if(retVal >= 0)
		{
			retVal -= (65535.0f * 0.5f);
			retVal /= 32767.0f;
		}
		else
			retVal = 0;
	}

	return retVal;
}
bool XBOXController::isDPadPointing(int val)
{
	JOYINFOEX _joyInfoEx;
	_joyInfoEx.dwSize = sizeof(_joyInfoEx);
	_joyInfoEx.dwFlags = JOY_RETURNPOVCTS;

	if(joyGetPosEx(joyID, &_joyInfoEx) == JOYERR_NOERROR)
	{
		if(_joyInfoEx.dwPOV == (unsigned int)val)
		{
			return true;
		}
	}
	return false;
}

void XBOXController::manualUpdateValues(float c1x,float c1y,float c2x,float c2y)
{
	if(c1x != _errVal)
		_c1X = c1x;
	if(c1y != _errVal)
		_c1Y = c1y;
	if(c2x != _errVal)
		_c2X = c2x;
	if(c2y != _errVal)
		_c2Y = c2y;
}

XBOXController::~XBOXController(void)
{
}
