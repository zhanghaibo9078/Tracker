#include "stdafx.h"
#include "cameraImaging.h"


cameraImaging::cameraImaging(){}

cameraImaging::cameraImaging(CDC *p)
{
	m_isOpen = false;
	isWork = false;
	isShow = false;
	width = 4096;
	height = 3072;
	imageBuffer = new uchar[width*height];
	showBuf = new uchar[width * height * 3];
	fps = 13;
	type = 'I';
	pBmp = (BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER)]);
	pBmp->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBmp->bmiHeader.biWidth = width;
	pBmp->bmiHeader.biHeight = height;
	pBmp->bmiHeader.biPlanes = 1;
	pBmp->bmiHeader.biBitCount = 24;
	pBmp->bmiHeader.biCompression = BI_RGB;
	pBmp->bmiHeader.biSizeImage = width * height * 3;
	pDC = p;
	imagingBuffer = new cv::Mat[1];
	imagingBuffer->create(height, width, CV_8U);
}


cameraImaging::~cameraImaging()
{
	if (m_isOpen)
	{
		Fg_stopAcquireEx(fg, nCamPort, pMem0, 0);
		Fg_FreeMemEx(fg, pMem0);
		Fg_FreeGrabber(fg);
		imagingBuffer->release();
		m_isOpen = false;
	}
}

bool cameraImaging::open()
{
	if (m_isOpen)
		return true;

	try
	{
		int nr_of_buffer = 1;
		int nBoard = 0;

		const char *dllName = "DualAreaGray16";
		if ((fg = Fg_Init(dllName, nBoard)) == NULL)
			return false;

		if (Fg_setParameter(fg, FG_WIDTH, &width, nCamPort) < 0)
			return false;
		if (Fg_setParameter(fg, FG_HEIGHT, &height, nCamPort) < 0)
			return false;
		int bitAlignment = FG_LEFT_ALIGNED;
		if (Fg_setParameter(fg, FG_BITALIGNMENT, &bitAlignment, nCamPort) < 0)
			return false;

		int nTapFormat = FG_CL_DUALTAP_8_BIT;
		int nUseDval = FG_YES;
		int nPixelformat = FG_GRAY;

		if (Fg_setParameter(fg, FG_CAMERA_LINK_CAMTYP, &nTapFormat, nCamPort)<0)
			return false;
		if (Fg_setParameter(fg, FG_USEDVAL, &nUseDval, nCamPort)<0)
			return false;
		if (Fg_setParameter(fg, FG_FORMAT, &nPixelformat, nCamPort)<0)
			return false;

		// Memory allocation
		int format;
		Fg_getParameter(fg, FG_FORMAT, &format, nCamPort);
		size_t bytesPerPixel = 1;
		switch (format) {
		case FG_GRAY:	bytesPerPixel = 1; break;
		case FG_GRAY16:	bytesPerPixel = 2; break;
		case FG_COL24:	bytesPerPixel = 3; break;
		case FG_COL32:	bytesPerPixel = 4; break;
		case FG_COL30:	bytesPerPixel = 5; break;
		case FG_COL48:	bytesPerPixel = 6; break;
		}
		size_t totalBufSize = width*height*nr_of_buffer * 1;
		if ((pMem0 = Fg_AllocMemEx(fg, totalBufSize, nr_of_buffer)) == NULL)
			return false;
		if ((Fg_AcquireEx(fg, nCamPort, GRAB_INFINITE, ACQ_STANDARD, pMem0)) < 0) {
			return false;
		}

		m_isOpen = true;
		return true;
	}
	catch (int e)
	{
		return false;
	}
}
bool cameraImaging::close()
{
	return true;
}
bool cameraImaging::getData()
{
	try
	{
		if (!m_isOpen)
			open();
		imageBuffer = (uchar*)Fg_getImagePtrEx(fg, Fg_getLastPicNumberEx(fg, 0, pMem0), 0, pMem0);
		return true;
	}
	catch (int e)
	{
		return false;
	}
}
void cameraImaging::show()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j<width; j++)
		{
			showBuf[i*width * 3 + j * 3 + 0] = imageBuffer[i*width + j];
			showBuf[i*width * 3 + j * 3 + 1] = imageBuffer[i*width + j];
			showBuf[i*width * 3 + j * 3 + 2] = imageBuffer[i*width + j];
		}
	StretchDIBits(pDC->m_hDC, 0, 0, width*0.2, height*0.2, 0, 0, width, height, showBuf, pBmp, DIB_RGB_COLORS, SRCCOPY);
}