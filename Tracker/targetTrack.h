#pragma once
#include "Camera.h"
class targetTrack
{
public:
	targetTrack();
	~targetTrack();
	void track(Camera *cam, float *cenX, float *cenY);
	byte *trackBuffer = NULL;
};

