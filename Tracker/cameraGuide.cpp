#include "stdafx.h"
#include "cameraGuide.h"

cameraGuide::cameraGuide() {}

cameraGuide::cameraGuide(CDC *p)
{
	m_isOpen = false;
	isWork = false;
	isShow = false;
	width = 2328;
	height = 1750;
	imageBuffer = new uchar[width*height];
	showBuf = new uchar[width * height * 3];
	capBuffer = new byte[(width+2) * height];
	fps = 26;
	type = 'G';
	pBmp = (BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER)]);
	pBmp->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBmp->bmiHeader.biWidth = width;
	pBmp->bmiHeader.biHeight = height;
	pBmp->bmiHeader.biPlanes = 1;
	pBmp->bmiHeader.biBitCount = 24;
	pBmp->bmiHeader.biCompression = BI_RGB;
	pBmp->bmiHeader.biSizeImage = width * height * 3;
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
			capBuffer = (uchar *)m_ptrGrabResult->GetBuffer();
			for (int i = 0; i < height; i++)
				memcpy(imageBuffer + i * width, capBuffer + i * (width+2) + 1, width);
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
		for(int j=0;j<width;j++)
		{
			showBuf[i*width * 3 + j * 3 + 0] = imageBuffer[i*width + j];
			showBuf[i*width * 3 + j * 3 + 1] = imageBuffer[i*width + j];
			showBuf[i*width * 3 + j * 3 + 2] = imageBuffer[i*width + j];
		}
	int margin = 60;
	int paintX = centerX-1, paintY = centerY-1;
	if (paintX > 0 && paintX < width && paintY>0 && paintY < height)
	{
		for (int bord = 0; bord < 10; bord++)
		{
			for (int i = paintY - margin; i < paintY + margin; i++)
			{
				if (paintX - margin > 0 && paintX - margin < width && i>0 && i < height)
				{
					showBuf[i*width * 3 + (paintX - margin + bord) * 3 + 0] = 0;
					showBuf[i*width * 3 + (paintX - margin + bord) * 3 + 1] = 0;
					showBuf[i*width * 3 + (paintX - margin + bord) * 3 + 2] = 255;
				}
				if (paintX + margin > 0 && paintX + margin < width && i>0 && i < height)
				{
					showBuf[i*width * 3 + (paintX + margin - bord) * 3 + 0] = 0;
					showBuf[i*width * 3 + (paintX + margin - bord) * 3 + 1] = 0;
					showBuf[i*width * 3 + (paintX + margin - bord) * 3 + 2] = 255;
				}
			}
			for (int i = paintX - margin; i < paintX + margin; i++)
			{
				if (i > 0 && i < width && paintY - margin>0 && paintY - margin < height)
				{
					showBuf[(paintY - margin + bord)*width * 3 + i * 3 + 0] = 0;
					showBuf[(paintY - margin + bord)*width * 3 + i * 3 + 1] = 0;
					showBuf[(paintY - margin + bord)*width * 3 + i * 3 + 2] = 255;
				}
				if (i > 0 && i < width && paintY + margin>0 && paintY + margin < height)
				{
					showBuf[(paintY + margin - bord)*width * 3 + i * 3 + 0] = 0;
					showBuf[(paintY + margin - bord)*width * 3 + i * 3 + 1] = 0;
					showBuf[(paintY + margin - bord)*width * 3 + i * 3 + 2] = 255;
				}
			}
		}
	}
	StretchDIBits(pDC->m_hDC, 0, 16, width*0.225, -height*0.225, 0, 0, width, height, showBuf, pBmp, DIB_RGB_COLORS, SRCCOPY);
}