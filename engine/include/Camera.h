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
#ifndef CAMERA_H_
#define CAMERA_H_

#include <GameObject.h>

namespace yam2d
{
	


/**
 * Class for Camera.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class Camera : public GameObject
{
public:
	// Default constructor.
	Camera(int gameObjectType)
		: GameObject(gameObjectType)
		, m_screenWidth(0)
		, m_screenHeight(0)
		, m_desiredAspectRatio(0.0f)
	{
	}

	virtual ~Camera() {}

	void setScreenSize(int screenWidth, int screenHeight )
	{
		setScreenSize(screenWidth,screenHeight,float(screenHeight));
	}

	void setScreenSize(int screenWidth, int screenHeight,  float screenUnitSize )
	{
		setScreenSize(screenWidth, screenHeight, screenUnitSize, float(screenWidth)/float(screenHeight) );
	}

	void setScreenSize(int screenWidth, int screenHeight,  float screenUnitSize, float desiredAspectRatio )
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		m_desiredAspectRatio = desiredAspectRatio;
		m_screenUnitSize = screenUnitSize;
	}

	virtual void render( Layer* layer);
	
	float getScaleFactorY()
	{
		float virtualY = m_screenUnitSize;
		float actualY = float(m_screenHeight);
	//	float desiredAspectRatio = m_desiredAspectRatio;
	//	float actualAspectRatio = float(m_screenWidth)/float(m_screenHeight);
	//	float virtualX = m_desiredAspectRatio*m_screenUnitSize;
	//	return (virtualY/actualY);// * 1.5f;// / actualAspectRatio;
		return virtualY/actualY;
		//return actualAspectRatio;
	}

	float getScaleFactorX()
	{
	//return 1.0f;
		return m_desiredAspectRatio*m_screenUnitSize/m_screenWidth;
	}

private:
	int m_screenWidth;
	int m_screenHeight;
	float m_desiredAspectRatio;
	float m_screenUnitSize;

	Camera();
	Camera(const Camera& o);
	Camera& operator=(const Camera& o);
};

}

#endif




