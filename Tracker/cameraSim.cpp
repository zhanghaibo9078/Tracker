#include "stdafx.h"
#include "cameraSim.h"

cameraSim::cameraSim() {}

cameraSim::cameraSim(CDC *p)
{
	m_isOpen = false;
	isWork = false;
	isShow = false;
	width = 4096;
	height = 3072;
	imageBuffer = new uchar[width*height];
	showBuf = new uchar[width * height * 3];
	fps = 13;
	type = 'S';
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


cameraSim::~cameraSim()
{
	close();
}

bool cameraSim::open()
{
	spaceStation = cv::imread("station.jpg");
	cv::Mat ROI = spaceStation.clone();
	cv::cvtColor(ROI, spaceStation, CV_RGB2GRAY);
	return true;
}
bool cameraSim::close()
{
	return true;
}
bool cameraSim::getData()
{
	static int row_prev = 0;
	static int col_prev = 0;
	int row = height / 2 + height*cos((double)timestamp / 100) / 4;
	int col = width / 2 + height*sin((double)timestamp / 100) / 4;
	if (timestamp < 1)
		memset(imageBuffer, 0, width*height);
	for (int i = 0; i<spaceStation.rows; i++)
	{
		memset(imageBuffer + (i + row_prev)*width + col_prev, 0, spaceStation.cols);
	}

	for (int i = 0; i<spaceStation.rows; i++)
	{
		memcpy(imageBuffer + (i + row)*width + col, spaceStation.data + i*spaceStation.cols, spaceStation.cols);
	}
	timestamp++;

	return true;
}
void cameraSim::show()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j<width; j++)
		{
			showBuf[i*width * 3 + j * 3 + 0] = imageBuffer[i*width + j];
			showBuf[i*width * 3 + j * 3 + 1] = imageBuffer[i*width + j];
			showBuf[i*width * 3 + j * 3 + 2] = imageBuffer[i*width + j];
		}
	StretchDIBits(pDC->m_hDC, 0, 0, width*0.13, -height*0.13, 0, 0, width, height, showBuf, pBmp, DIB_RGB_COLORS, SRCCOPY);
}