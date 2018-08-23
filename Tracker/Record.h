#pragma once
#include "Camera.h"

class Record
{
public:
	Record();
	Record(char c, int w, int h, int f);
	~Record();
	cv::Mat *frame;
	void stop();
	void write(uchar *buffer);
private:
	char type;
	int width = 0;
	int height = 0;
	int frameRate = 1;
	cv::VideoWriter *videoW = NULL;
};