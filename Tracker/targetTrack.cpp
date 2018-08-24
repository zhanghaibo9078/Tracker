#include "stdafx.h"
#include "targetTrack.h"


targetTrack::targetTrack()
{
}


targetTrack::~targetTrack()
{
}

void targetTrack::track(Camera *cam,float *cenX,float *cenY)
{
	//FILE *f = fopen("1.raw", "wb+");
	//fwrite(cam->imageBuffer, 1, cam->width*cam->height, f);
	//fclose(f);
	//if(trackBuffer==NULL)
	//	trackBuffer = new byte[cam->width*cam->height];
	int row = 0, col = 0,value = 0;
	int x = 0, y = 0,cntX=0,cntY;
	for (row = 0; row<cam->height; row++)
		for (col = 0; col < cam->width; col++)
		{
			value = cam->imageBuffer[row*cam->width + col];
			if (cam->imageBuffer[row*cam->width + col] > 200)
			{
				x += col*value;
				y += row*value;
				cntX += value;
			}
		}
	if (cntX < 1)
	{
		cam->centerX = 0;
		cam->centerY = 0;
		*cenX = 0;
		*cenY = 0;
	}
	else
	{
		cam->centerX = x * 1.0 / cntX;
		cam->centerY = y * 1.0 / cntX;
		*cenX = cam->centerX - (cam->width-1)/2;
		*cenY = (cam->height-1)/2 - cam->centerY;
	}
	//for (row = 0; row<cam->height; row++)
	//	for (col = 0; col < cam->width; col++)
	//	{
	//		x += col*buf[row*cam->width + col];
	//		y += row*buf[row*cam->width + col];
	//		if (buf[row*cam->width + col] > 0)
	//		{
	//			cntX += buf[row*cam->width + col];
	//		}
	//	}
	//cv::Mat tempImg(cam->height, cam->width, CV_8UC1, cam->imageBuffer, cam->width);
	//cv::Mat tempImg = cv::imread("1.bmp");
	//cv::Mat poseImg;
	//cv::threshold(tempImg, poseImg, 200, 0, CV_THRESH_TOZERO);
	//cv::imwrite("1.bmp", tempImg);
	//cv::imwrite("2.bmp", poseImg);
	//cv::Moments mom = cv::moments(poseImg);
	//»æÖÆÖÊÐÄ
	//cv::circle(result, cv::Point(mom.m10 / mom.m00, mom.mom01 / mom.mom00), 2, cv::Scalar(0), 2);
	//IplImage tmp = IplImage(poseImg);
	//CvArr* arr = (CvArr*)&tmp;
	//CvMoments *moment = (CvMoments*)malloc(sizeof(CvMoments));
	//cvMoments(arr,moment,0);
	//double m00 = cvGetSpatialMoment(moment, 0, 0);
	//double m10 = cvGetSpatialMoment(moment, 1, 0);
	//double m01 = cvGetSpatialMoment(moment, 0, 1);
	//center.x = (int)(m10 / m00);
	//center.y = (int)(m01 / m00);
	//int value = 0;
	//for (int i = 0; i<poseImg.rows; i++)
	//	for (int j = 0; j<poseImg.rows; j++)
	//	{
	//		cvGet2D(poseImg, j, i)
	//		value = poseImg.row(i).col(j);
	//	}
	////cv::blur(tempImg, poseImg, cv::Size(5, 5));
	////cv::GaussianBlur(tempImg, poseImg, cv::Size(3, 3), 0.1, 0);
	//tempImg = tempImg - poseImg-40;
	//cv::imwrite("3.bmp", tempImg);
	//cv::Moments mom = cv::moments(poseImg);
	//cv::circle(tempImg, cv::Point(mom.m10 / mom.m00, mom.m01 / mom.m00), 2, cv::Scalar(0), 2);
	//cv::imwrite("4.bmp", tempImg);
	////cv::threshold(tempImg, poseImg, 100, 200, CV_THRESH_BINARY);
	////cv::dilate(poseImg, tempImg, cv::Mat(), cv::Point(1, 1), 20);
	////cv::imwrite("4.bmp", tempImg);
	////cv::imwrite("5.bmp", poseImg);
	////int a = cv::connectedComponents(poseImg, tempImg);
	//////cv::Canny(poseImg, tempImg, 100, 255);
	//////    cv::Mat erodeStruct = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
	//////    cv::morphologyEx(poseImg,tempImg,cv::MORPH_CLOSE,cv::Mat());
	//////    cv::erode(tempImg,poseImg,cv::Mat());
	////cv::imwrite("6.bmp", tempImg);
	////cv::imwrite("7.bmp", poseImg);
	////    cv::dilate(tempImg,poseImg,erodeStruct,cv::Point(-1,-1),2);
	////    cv::dilate(tempImg,poseImg,erodeStruct);
	//cam->imageBuffer = poseImg.data;
}