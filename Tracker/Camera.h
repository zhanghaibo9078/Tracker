#pragma once
#include "stdafx.h"
#include <pylon/PylonIncludes.h>
#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>

using namespace Pylon;

class Camera
{
public:
	Camera() {};
	virtual ~Camera() {};

	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual bool getData() = 0;
	virtual void show() = 0;
	
	CDC *pDC = NULL;
	BITMAPINFO *pBmp = NULL;
	bool m_isOpen = false;
	bool isWork = false;
	bool isShow = false;

	UCHAR *imageBuffer;
	int width = 0;
	int height = 0;
	int fps = 0;
	char type = 0;
};

