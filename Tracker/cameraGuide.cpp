#include "stdafx.h"
#include "cameraGuide.h"

cameraGuide::cameraGuide() {}

cameraGuide::cameraGuide(CDC *p)
{
	m_isOpen = false;
	isWork = false;
	isShow = false;
	width = 2330;
	showWidth = 2328;
	height = 1750;
	imageBuffer = new uchar[width*height];
	showBuf = new uchar[showWidth * height * 3];
	fps = 26;
	type = 'G';
	pBmp = (BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER)]);
	pBmp->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBmp->bmiHeader.biWidth = showWidth;
	pBmp->bmiHeader.biHeight = height;
	pBmp->bmiHeader.biPlanes = 1;
	pBmp->bmiHeader.biBitCount = 24;
	pBmp->bmiHeader.biCompression = BI_RGB;
	pBmp->bmiHeader.biSizeImage = showWidth * height * 3;
	pDC = p;
}


cameraGuide::~cameraGuide()
{
	close();
}

bool cameraGuide::open()
{
	try
	{
		if (!m_isOpen)
		{
			PylonInitialize();
			m_camera = new CInstantCamera(CTlFactory::GetInstance().CreateFirstDevice());
			m_camera->MaxNumBuffer = 5;
			m_camera->StartGrabbing();
			m_isOpen = true;
		}
		return true;
	}
	catch (const GenericException &e)
	{
		return false;
	}
}
bool cameraGuide::close()
{
	if (m_isOpen)
	{
		m_camera->Close();
		m_isOpen = false;
	}
	return true;
}
bool cameraGuide::getData()
{
	try
	{
		if (!m_isOpen)
			open();
		m_camera->RetrieveResult(5000, m_ptrGrabResult);
		if (m_ptrGrabResult->GrabSucceeded())
		{
			imageBuffer = (uchar *)m_ptrGrabResult->GetBuffer();
		}
		return true;
	}
	catch (int e)
	{
		return false;
	}
}
void cameraGuide::show()
{
	for (int i = 0; i < height; i++)
		for(int j=0;j<showWidth;j++)
		{
			showBuf[i*showWidth * 3 + j * 3 + 0] = imageBuffer[i*width + j + 1];
			showBuf[i*showWidth * 3 + j * 3 + 1] = imageBuffer[i*width + j + 1];
			showBuf[i*showWidth * 3 + j * 3 + 2] = imageBuffer[i*width + j + 1];
		}
	StretchDIBits(pDC->m_hDC, 0, 0, showWidth*0.2, height*0.2, 0, 0, showWidth, height, showBuf, pBmp, DIB_RGB_COLORS, SRCCOPY);
}