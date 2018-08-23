#pragma once
#include "Camera.h"
class cameraImaging : public Camera
{
public:
	cameraImaging();
	cameraImaging(CDC *p);
	~cameraImaging();

	virtual bool open();
	virtual bool close();
	virtual bool getData();
	virtual void show();
private:
	Fg_Struct *fg;
	dma_mem * pMem0;
	int nCamPort = PORT_A;		// Port (PORT_A / PORT_B)
	cv::Mat *imagingBuffer;

	uchar *showBuf;
};

