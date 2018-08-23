#pragma once
#include "Camera.h"
class cameraGuide : public Camera
{
public:
	cameraGuide();
	cameraGuide(CDC *p);
	~cameraGuide();

	virtual bool open();
	virtual bool close();
	virtual bool getData();
	virtual void show();
private:
	CGrabResultPtr m_ptrGrabResult;
	CInstantCamera *m_camera;
	uchar *showBuf;
	int showWidth = 0;
};

