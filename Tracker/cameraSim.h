#pragma once
#include "Camera.h"
class cameraSim : public Camera
{
public:
	cameraSim();
	cameraSim(CDC *p);
	~cameraSim();

	virtual bool open();
	virtual bool close();
	virtual bool getData();
	virtual void show();
private:
	UCHAR *showBuf;
	cv::Mat spaceStation;
	int timestamp = 0;
};

