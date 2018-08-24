#include "stdafx.h"
#include "cameraSim.h"

cameraSim::cameraSim() {}

cameraSim::cameraSim(CDC *p)
{
	m_isOpen = false;
	isWork = false;
	isShow = false;
	width = 2328;
	height = 1750;
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
	int margin = 50;
	int paintX = centerX, paintY = centerY;
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
	StretchDIBits(pDC->m_hDC, 0, 0, width*0.225, -height*0.225, 0, 0, width, height, showBuf, pBmp, DIB_RGB_COLORS, SRCCOPY);
}