#pragma once
#include "stdafx.h"
#include <pylon/PylonIncludes.h>
#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>

#include <clser.h>
#include <os_type.h>
#include <fgrab_struct.h>
#include <fgrab_prototyp.h>
#include <fgrab_define.h>
#include <SisoDisplay.h>

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

	uchar *imageBuffer;
	int width = 0;
	int height = 0;
	int fps = 0;
	char type = 0;
};

