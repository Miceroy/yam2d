#include "../../include/XBOXController.h"
#include "vec2.h"
#include <es_util.h>



XBOXController::XBOXController(int controllerIndex)
	: m_joyID(JOYSTICKID1 + controllerIndex)
	, m_connected(false)
{
}


XBOXController::~XBOXController(void)
{
	if (joyReleaseCapture(m_joyID))
	{
		yam2d::esLogMessage("Joystick(%d) release fails", m_joyID);
	}
}


bool XBOXController::isConnected()
{
	if (!m_connected)
	{
		UINT numDevs = joyGetNumDevs();
		UINT selected = numDevs;
		m_connected = true;
		for (UINT i = 0; i < numDevs; ++i)
		{
			JOYCAPS caps;
			if (JOYERR_NOERROR == joyGetDevCaps(i, &caps, sizeof(caps)))
			{
				//yam2d::esLogMessage("Joystick(%d): %s", i, caps.szPname);
				if (i == m_joyID)
				{
					selected = i;
					m_caps = caps;
				}
			}
		}

		if (selected == numDevs)
		{
			m_connected = false;
		}

		yam2d::ESContext* ctx = yam2d::esGetCurrentContext();
		if (joySetCapture(ctx->hWnd, m_joyID, 0, FALSE))
		{
			m_connected = false;
		}
	}
	
	if (m_connected)
	{
		joyInfoEx.dwSize = sizeof(joyInfoEx);
		joyInfoEx.dwFlags = JOY_RETURNALL;
		m_prevJoyInfoEx = joyInfoEx;
		MMRESULT res = joyGetPosEx(m_joyID, &joyInfoEx);
		m_connected = false;
		if (JOYERR_NOERROR == res)
		{
			m_connected = true;
		}
		else if (MMSYSERR_NODRIVER == res)
		{
			yam2d::esLogMessage("Joystick(%d) The joystick driver is not present.", m_joyID);
		}
		else if (MMSYSERR_INVALPARAM == res)
		{
			yam2d::esLogMessage("Joystick(%d) An invalid parameter was passed.", m_joyID);
		}
		else if (MMSYSERR_BADDEVICEID == res)
		{
			yam2d::esLogMessage("Joystick(%d) The specified joystick identifier is invalid.", m_joyID);
		}
		else if (JOYERR_UNPLUGGED == res)
		{
			yam2d::esLogMessage("Joystick(%d) The specified joystick is not connected to the system.", m_joyID);
		}
		else if (JOYERR_PARMS == res)
		{
			yam2d::esLogMessage("Joystick(%d) The specified joystick identifier is invalid.", m_joyID);
		}
		else
		{
			yam2d::esLogMessage("Joystick(%d) unknown error", m_joyID);
		}
	}

	return m_connected;
}


bool XBOXController::isButtonDown(JoyButtons button)
{
	return (joyInfoEx.dwButtons & button) ? true : false;
}

bool XBOXController::isButtonPressed(JoyButtons button)
{
	bool curPressed = isButtonDown(button);
	bool prevPressed = (m_prevJoyInfoEx.dwButtons & button) ? true : false;

	return curPressed && !prevPressed;
}

bool XBOXController::isButtonReleased(JoyButtons button)
{
	bool curPressed = isButtonDown(button);
	bool prevPressed = (m_prevJoyInfoEx.dwButtons & button) ? true : false;

	return !curPressed && prevPressed;
}

float mapValue(UINT val, UINT min, UINT max)
{
	float delta = (float)(max - min);
	float v = (float)(val - min);
	float res = v / delta; // res == 0.0f - 1.0f
	return res * 2.0f - 1.0f; // return -1.0f - 1.0f
}

float XBOXController::getAxis(JoyAxis i)
{
	if ((i == ZAxis) && ((m_caps.wCaps&JOYCAPS_HASZ) == 0))
		return 0.0f;
	
	if ((i == RAxis) && ((m_caps.wCaps&JOYCAPS_HASR) == 0))
		return 0.0f;

	if ((i == UAxis) && ((m_caps.wCaps&JOYCAPS_HASU) == 0))
		return 0.0f;

	if ((i == VAxis) && ((m_caps.wCaps&JOYCAPS_HASV) == 0))
		return 0.0f;

	float res = 0.0f;

	switch (i)
	{
	case XAxis:
		res = mapValue(joyInfoEx.dwXpos, m_caps.wXmin, m_caps.wXmax);
		break;
	case YAxis:
		res = mapValue(joyInfoEx.dwYpos, m_caps.wYmin, m_caps.wYmax);
		break;
	case ZAxis:
		res = mapValue(joyInfoEx.dwZpos, m_caps.wZmin, m_caps.wZmax);
		break;
	case RAxis:
		res = mapValue(joyInfoEx.dwRpos, m_caps.wRmin, m_caps.wRmax);
		break;
	case UAxis:
		res = mapValue(joyInfoEx.dwUpos, m_caps.wUmin, m_caps.wUmax);
		break;
	case VAxis:
		res = mapValue(joyInfoEx.dwVpos, m_caps.wVmin, m_caps.wVmax);
		break;
	default:
		break;
	}

	return res;
}


void XBOXController::updateAllAxis()
{

}



bool XBOXController::isDPadPointing(DPadDir val)
{
	if ((m_caps.wCaps&JOYCAPS_HASPOV) == 0)
	{
		return false;
	}

	if ((m_caps.wCaps&JOYCAPS_POV4DIR) == 0)
	{
		return false;
	}

	return (joyInfoEx.dwPOV == (unsigned int)val) ? true : false;
}

bool XBOXController::isDPadPressed()
{

	if ((m_caps.wCaps&JOYCAPS_HASPOV) == 0)
	{
		return false;
	}

	if (joyInfoEx.dwPOV >= 36000)
	{
		return false;
	}

	return true;
}

float XBOXController::getDPadAngle()
{
	if (joyInfoEx.dwPOV < 0)
		return -1.0f;

	float curValue = ((float)joyInfoEx.dwPOV) * 0.01f;
	return curValue;
}

float XBOXController::getDPadNewAngle()
{
	float curValue = getDPadAngle();
	float prevValue = ((float)m_prevJoyInfoEx.dwPOV) * 0.01f;
	if (curValue == prevValue)
		return -1.0f;

	return curValue;
}
