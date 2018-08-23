#include "stdafx.h"
#include "record.h"

Record::Record() {}

Record::Record(char s, int w, int h, int f)
{
	type = s;
	width = w;
	height = h;
	frameRate = f;
	frame = new cv::Mat(height, width, CV_8UC1);
}

Record::~Record() {}

void Record::write(uchar *buffer)
{
	if (videoW == NULL)
	{
		CString fName = CTime::GetCurrentTime().Format("%Y%m%d_%H%M%S");
		fName.Format(_T("%s_%c.avi"), fName, type);
		videoW = new cv::VideoWriter();
		USES_CONVERSION;
		videoW->open(W2A(fName), CV_FOURCC('D', 'I', 'V', 'X'), frameRate, cv::Size(width, height), false);
	}
	frame->data = buffer;
	videoW->write(*frame);
}

void Record::stop()
{
	if (videoW != NULL)
	{
		videoW->release();
		videoW = NULL;
	}
}
